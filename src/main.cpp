#include <memory>
#include <iostream>
#include <fstream>
#include <utility> // for pair

#include <Eigen/Core>
#include <Eigen/Dense>

#include "soundfile.h"
#include "feature_extractor.h"

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


static const float kStepSize = 5e-3f;
static const float kWindowSize = 20e-3f;
static const float kEpsilon = 1e-5;

std::vector< std::pair<size_t, size_t> > align(const Eigen::MatrixXf &ref, const Eigen::MatrixXf &deg) {
    std::vector< std::pair<size_t, size_t> > path;

    Eigen::VectorXf ref_norm = ref.colwise().squaredNorm();
    Eigen::VectorXf deg_norm = deg.colwise().squaredNorm();
    Eigen::MatrixXf total_norm = deg_norm * ref_norm.transpose();
    total_norm = total_norm.array().sqrt();

    Eigen::MatrixXf correlation = (deg.transpose() * ref).array() / (total_norm.array() + kEpsilon);

    const size_t nRows =  correlation.rows();
    const size_t nCols =  correlation.cols();

    {
        std::ofstream ofs ("correlation.txt", std::ofstream::out);
        ofs << correlation;
        ofs.close();
    }

    Eigen::MatrixXf gamma = Eigen::MatrixXf::Zero(nRows, nCols);

    struct location_s *previous_location = (struct location_s *) calloc(sizeof(struct location_s), nRows * nCols );

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
    int m = nRows - 1;
    int n = nCols - 1;

    while(n > 0 || m > 0)
    {
        path.push_back(std::make_pair(m, n));
        size_t index = m * nCols + n;
        m = previous_location[index].x;
        n = previous_location[index].y;
    }
    path.push_back(std::make_pair(m, n));

    free(previous_location);
    return path;
}

int main(int argc, char **argv)
{
    if(argc < 3) {
        std::cerr << "Not enough parameter" << std::endl;
        std::cerr << "Usage: " << argv[0] << " reference_file degraded_file"  << std::endl;
        return 1;
    }

    auto reference = std::make_shared<SoundFile>(argv[1]);
    auto ref_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, kWindowSize, kStepSize));

    auto degraded = std::make_shared<SoundFile>(argv[2]);
    auto deg_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(degraded, kWindowSize, kStepSize));

    Eigen::MatrixXf ref_desc = ref_descriptors->descriptors();
    Eigen::MatrixXf deg_desc = deg_descriptors->descriptors();
    
    auto path = align(ref_desc, deg_desc);

    {
        std::ofstream path_file("path.txt", std::ofstream::out);
        for(std::pair<size_t, size_t> &correspondance: path)
        {
            path_file << correspondance.first * kStepSize << " " << correspondance.second * kStepSize << " " << correspondance.first << " " << correspondance.second << std::endl;
        }
    }

    return 0;
}

