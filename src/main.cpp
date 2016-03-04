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
    
    Eigen::MatrixXf correlation = ref_desc.transpose() * deg_desc;
    
    std::cout << correlation;
    return 0;
}

