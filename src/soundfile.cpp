#include <stdint.h>
#include <assert.h>
#include <string.h>

#include <vector>

#include "soundfile.h"

#include "sndfile.h"


SoundFile::SoundFile(std::string filename)
    : soundfile_(nullptr)
{
    soundfile_ = sf_open(filename.c_str(), SFM_READ, &file_info_);
    assert(soundfile_ != nullptr);
    assert(file_info_.channels == 1); // We do not support multichannel audio files
}

SoundFile::~SoundFile()
{
    if (soundfile_) {
        sf_close(soundfile_);
    }
}

size_t SoundFile::getFrame(size_t length, float* frame)
{
    size_t count = 0;
    count = sf_readf_float(soundfile_, frame, length);
    return count;
}
