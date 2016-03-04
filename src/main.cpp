#include <memory>
#include <iostream>

#include "soundfile.h"
#include "feature_extractor.h"


int main(int argc, char **argv)
{
	std::cout << "Sound Alignement" << std::endl;

    auto reference = std::make_shared<SoundFile>("ref.wav");
    auto degraded = std::make_shared<SoundFile>();

    auto ref_descriptors = std::make_unique<PSDFeature>(reference, 20e-3f, 10e-3f);
    ref_descriptors->descriptors();

    return 0;
}

