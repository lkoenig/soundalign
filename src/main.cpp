#include <memory>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "soundfile.h"
#include "feature_extractor.h"



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
    return 0;
}

