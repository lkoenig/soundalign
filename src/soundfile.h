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

    int getSampleRate() const {
        return file_info_.samplerate;
    }

    uint64_t getNumberOfSamples() const {
        return file_info_.frames;
    }

    size_t getNumberOfChannel() const {
        return (size_t) file_info_.channels;
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
