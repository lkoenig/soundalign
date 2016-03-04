#include <memory>
#include <iostream>

#include "soundfile.h"
#include "feature_extractor.h"


int main(int argc, char **argv)
{
    std::cout << "Sound Alignement" << std::endl;

    auto reference = std::make_shared<SoundFile>("ref.wav");
    
    auto ref_descriptors = std::unique_ptr<PSDFeature>(new PSDFeature(reference, 20e-3f, 10e-3f));
    ref_descriptors->descriptors();

    return 0;
}

