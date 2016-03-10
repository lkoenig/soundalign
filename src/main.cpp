#include <memory>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "soundfile.h"
#include "feature_extractor.h"


template<typename T> T min(T a, T b, T c) {
    if (a < b && a < c) return a;
    if (b < a && b < c) return b;
    if (c < a && c < b) return c;
    assert(0 && "Should never reach that part");
    return 0;
}


int main(int argc, char **argv)
{
    std::cout << "Sound Alignement" << std::endl;

    auto reference = std::make_shared<SoundFile>("ref.wav");    
    auto ref_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, 20e-3f, 10e-3f));
    auto degraded = std::make_shared<SoundFile>("ref.wav");
    auto deg_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, 20e-3f, 10e-3f));

    Eigen::MatrixXf ref_desc = ref_descriptors->descriptors();
    Eigen::MatrixXf deg_desc = ref_descriptors->descriptors();
    
    Eigen::VectorXf ref_norm = ref_desc.colwise().squaredNorm();
    Eigen::VectorXf deg_norm = deg_desc.colwise().squaredNorm();
    Eigen::MatrixXf total_norm = ref_norm * deg_norm.transpose();
    total_norm = total_norm.array().sqrt();

    Eigen::MatrixXf correlation = (ref_desc.transpose() * deg_desc).array() / total_norm.array();

    
    Eigen::MatrixXf gamma = Eigen::MatrixXf::Zero(correlation.rows(), correlation.cols());
    for (int n = 1; n < correlation.rows(); n++) {
        gamma(n, 0) = 1e45;
    }
    for (int m = 1; m < correlation.cols(); m++) {
        gamma(0, m) = 1e45;
    }
    
    assert( (gamma(0, 0) == 0.f) );

    for (int n = 1; n < correlation.cols(); n++) {
        for (int m = 1; m < correlation.rows(); m++) {
            gamma(n,m) = correlation(n, m) + min(gamma(n-1, m), gamma(n-1, m-1), gamma(n, m-1));
        }
    }

    // Traceback

    return 0;
}

