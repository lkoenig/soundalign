#include <memory>
#include <iostream>
#include <utility> // for pair

#include <fstream>


#include <Eigen/Core>
#include <Eigen/Dense>

#include "soundfile.h"
#include "feature_extractor.h"

#include "align.h"



static const float kStepSize = 5e-3f;
static const float kWindowSize = 20e-3f;


int main(int argc, char **argv)
{
    if(argc < 3) {
        std::cerr << "Not enough parameter" << std::endl;
        std::cerr << "Usage: " << argv[0] << " reference_file degraded_file"  << std::endl;
        return 1;
    }

    auto reference = std::make_shared<SoundFile>(argv[1]);

    auto degraded = std::make_shared<SoundFile>(argv[2]);

    if(degraded->getSampleRate() != reference->getSampleRate()) {
        std::cerr << "Reference file and dergraded files *have to* have the same sampling frequency: " << reference->getSampleRate() << " != " << degraded->getSampleRate() << std::endl;
        return 1;
    }

    if(degraded->getNumberOfChannel() != 1 || reference->getNumberOfChannel() != 1) {
        std::cerr << "Reference file and dergraded files *have to* be monophonic" << std::endl;
        return 1;
    }

    auto ref_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, kWindowSize, kStepSize));
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

