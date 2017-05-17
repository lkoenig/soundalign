#ifndef SOUNDFILE_H_
#define SOUNDFILE_H_

#include <vector>
#include <string>
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

    /**
     * @brief getFrame
     * @param length
     * @param buffer where the frames ends up
     * @return
     */
    size_t getFrame(size_t length, float* buffer);

    virtual ~SoundFile();
};

#endif // SOUNDFILE_H_
