#include "align.h"

#include <memory>

#if DEBUG_DUMP_DATA
#include <fstream>
#endif

static const float kEpsilon = 1e-5;

template<typename T> int argmax(T a, T b, T c) {
    if (a > b && a > c) return 0;
    if (b > a && b > c) return 1;
    if (c > a && c > b) return 2;
    return 0;
}

struct location_s {
    size_t x;
    size_t y;
};


std::vector< std::pair<size_t, size_t> > align(const Eigen::MatrixXf &ref, const Eigen::MatrixXf &deg) {
    std::vector< std::pair<size_t, size_t> > path;

    Eigen::VectorXf ref_norm = ref.colwise().squaredNorm();
    Eigen::VectorXf deg_norm = deg.colwise().squaredNorm();
    Eigen::MatrixXf total_norm = deg_norm * ref_norm.transpose();
    total_norm = total_norm.array().sqrt();

    Eigen::MatrixXf correlation = (deg.transpose() * ref).array() / (total_norm.array() + kEpsilon);

    const size_t nRows =  correlation.rows();
    const size_t nCols =  correlation.cols();

#if DEBUG_DUMP_DATA
    {
        std::ofstream ofs ("correlation.txt", std::ofstream::out);
        ofs << correlation;
        ofs.close();
    }
#endif

    Eigen::MatrixXf gamma = Eigen::MatrixXf::Zero(nRows, nCols);

    std::unique_ptr<location_s[]> previous_location(new location_s[nRows * nCols]);
    memset(previous_location.get(), 0, sizeof(previous_location[0]) * nRows * nCols);

    for (size_t n = 0; n < nRows; n++) {
        gamma(n, 0) = correlation(n,0);
    }
    for (size_t m = 1; m < nCols; m++) {
        gamma(0, m) = correlation(0,m);
    }

    // m row index, n col index

    for (size_t n = 1; n < nCols; n++) {
        for (size_t m = 1; m < nRows; m++) {
            int previous = argmax(gamma(m-1, n-1), gamma(m, n-1), gamma(m-1, n));
            switch(previous) {
            case 0:
                gamma(m, n) = correlation(m, n) + gamma(m-1, n-1);
                previous_location[m * nCols + n].x = m-1;
                previous_location[m * nCols + n].y = n-1;
                break;
            case 1:
                gamma(m, n) = correlation(m, n) + gamma(m, n-1);
                previous_location[m * nCols + n].x = m;
                previous_location[m * nCols + n].y = n-1;
                break;
            case 2:
                gamma(m, n) = correlation(m, n) + gamma(m-1, n);
                previous_location[m * nCols + n].x = m-1;
                previous_location[m * nCols + n].y = n;
                break;
            default:
                assert(0);
                break;
            }
        }
    }

    // Traceback
    size_t m = nRows - 1;
    size_t n = nCols - 1;

    while(n > 0 || m > 0)
    {
        path.emplace_back(m, n);
        size_t index = m * nCols + n;
        m = previous_location[index].x;
        n = previous_location[index].y;
    }
    path.emplace_back(m, n);

    return path;
}
