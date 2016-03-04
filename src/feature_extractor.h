#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <memory>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "kiss_fft.h"
#include "kiss_fftr.h"

#include "feature.h"
#include "circular_buffer.h"
#include "soundfile.h"

const int kNFFT = 1024;

class PSDFeature : public IFeature {
public:
    PSDFeature(std::shared_ptr<SoundFile> file, float window_length_s, float overlap_s);
    ~PSDFeature();

    Eigen::MatrixXf descriptors();


private:
    std::unique_ptr<CircularBuffer<float>> buffer_;
    std::shared_ptr<SoundFile> soundfile_;
    kiss_fftr_cfg fft_config_;
    float window_length_ms_;
    float step_ms_;
};

#endif
