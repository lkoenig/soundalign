#ifndef SOUNDFILE_H_
#define SOUNDFILE_H_

#include <vector>
#include "sndfile.h"

class SoundFile
{
    SNDFILE *soundfile_;
    SF_INFO file_info_;

public:
    SoundFile(std::string filename);

    int getSampleRate() {
        return file_info_.samplerate;
    }

    uint64_t getNumberOfSamples() {
        return file_info_.frames;
    }

    std::vector<float> && getFrame(size_t length);

    virtual ~SoundFile();
};

#endif // SOUNDFILE_H_
