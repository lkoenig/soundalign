#include <Eigen/Core>
#include <Eigen/Dense>

#include "kiss_fft.h"
#include "kiss_fftr.h"

#include "feature_extractor.h"

using namespace Eigen;

PSDFeature::PSDFeature(std::shared_ptr<SoundFile> file, float window_length_ms, float step)
    : soundfile_(file)
    , window_length_ms_(window_length_ms)
    , step_ms_(step)
{
    buffer_ = std::make_unique< CircularBuffer<float> >((size_t) (window_length_ms_ * soundfile_->getSampleRate()));
    fft_config_ = kiss_fftr_alloc(kNFFT, 0, NULL, 0);
}

PSDFeature::~PSDFeature() {
    kiss_fftr_free(fft_config_);
}


MatrixXf PSDFeature::descriptors()
{
    int window_length_in_sample = int(window_length_ms_ * soundfile_->getSampleRate());
    int step_in_sample = int(step_ms_ * soundfile_->getSampleRate());
    size_t n_frames = (size_t)soundfile_->getNumberOfSamples() / step_in_sample;

    MatrixXf desc(kNFFT / 2, n_frames);
    std::vector<float> window(window_length_in_sample);
    std::vector<kiss_fft_cpx> frequencies(kNFFT / 2 + 1);

    for (size_t frame_index = 0; frame_index < n_frames; frame_index++)
    {
        kiss_fftr(fft_config_, window.data(), frequencies.data());

        float *psd = desc.col(frame_index).data();
        for (auto& coef : frequencies) {
            (*psd++) = sqrtf(coef.i * coef.i + coef.r * coef.r);
        }
    }

    return desc;
}
