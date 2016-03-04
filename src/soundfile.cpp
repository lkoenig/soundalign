#include <iostream>
#include <vector>
#include <cstdint>

#include "soundfile.h"

#include "sndfile.h"

#include <kiss_fftr.h>

#include <Eigen/Core>
#include <Eigen/Dense>

static const int kNFFT = 1024;

using namespace Eigen;

SoundFile::SoundFile(std::string filename) 
{
    
    soundfile_ = sf_open(filename.c_str(), SFM_READ, &file_info_);
}

SoundFile::~SoundFile()
{
    if (soundfile_) {
        sf_close(soundfile_);
    }
}

std::vector<float> SoundFile::getFrame(size_t length)
{
    std::vector<float> frame(length);
    sf_readf_float(soundfile_, frame.data(), length);

    return frame;
}

