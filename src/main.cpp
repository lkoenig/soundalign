#include <memory>
#include <iostream>
#include <fstream>

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

int main(int argc, char **argv)
{
    auto reference = std::make_shared<SoundFile>("ref.wav");    
    auto ref_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, kWindowSize, kStepSize));

    auto degraded = std::make_shared<SoundFile>("ref.wav");
    auto deg_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(degraded, kWindowSize, kStepSize));

    Eigen::MatrixXf ref_desc = ref_descriptors->descriptors();
    Eigen::MatrixXf deg_desc = deg_descriptors->descriptors();
    
    Eigen::VectorXf ref_norm = ref_desc.colwise().squaredNorm();
    Eigen::VectorXf deg_norm = deg_desc.colwise().squaredNorm();
    Eigen::MatrixXf total_norm = ref_norm * deg_norm.transpose();
    total_norm = total_norm.array().sqrt();

    Eigen::MatrixXf correlation = (ref_desc.transpose() * deg_desc).array() / (total_norm.array() + 1e-3);

    const size_t nRows =  correlation.rows();
    const size_t nCols =  correlation.cols();

    {
        std::ofstream ofs ("correlation.txt", std::ofstream::out);
        ofs << correlation;
        ofs.close();
    }

    Eigen::MatrixXf gamma = Eigen::MatrixXf::Zero(nRows, nCols);

    struct location_s *previous_location = (struct location_s *) calloc(sizeof(struct location_s), nRows * nCols );

    for (int n = 0; n < nRows; n++) {
        gamma(n, 0) = correlation(n,0);
    }
    for (int m = 1; m < nCols; m++) {
        gamma(0, m) = correlation(0,m);
    }
    
    // m row index, n col index

    for (int n = 1; n < nCols; n++) {
        for (int m = 1; m < nRows; m++) {
            int previous = argmax(gamma(m-1, n-1), gamma(m, n-1), gamma(m-1, n));
            switch(previous) {
            case 0:
                gamma(n,m) = correlation(m, n) + gamma(m-1, n-1);
                previous_location[m * nCols + n].x = m-1;
                previous_location[m * nCols + n].y = n-1;
                break;
            case 1:
                gamma(n,m) = correlation(m, n) + gamma(m, n-1);
                previous_location[m * nCols + n].x = m;
                previous_location[m * nCols + n].y = n-1;
                break;
            case 2:
                gamma(n,m) = correlation(m, n) + gamma(m-1, n);
                previous_location[m * nCols + n].x = m-1;
                previous_location[m * nCols + n].y = n;
                break;
            default:
                assert(0);
                break;
            }
        }
    }

    {
        std::ofstream ofs ("gamma.txt", std::ofstream::out);
        ofs << gamma;
        ofs.close();
    }


    // Traceback
    int m = nRows - 1;
    int n = nCols - 1;
    std::ofstream path ("path.txt", std::ofstream::out);
    while(n > 0 || m > 0)
    {
        path << m * kStepSize << " " << n * kStepSize << " " << m << " " << n << std::endl;
        size_t index = m * nCols + n;
        m = previous_location[index].x;
        n = previous_location[index].y;
    }
    path.close();

    free(previous_location);
    return 0;
}

