#include <memory>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "kiss_fft.h"
#include "kiss_fftr.h"

#include "feature_extractor.h"

using namespace Eigen;

size_t next_pow_2(size_t n) {
    size_t output = 1;
    while (output < n) {
        output = output << 1;
    }
    return output;
}

PSDFeature::PSDFeature(std::shared_ptr<SoundFile> file, float window_length_ms, float step)
    : soundfile_(file)
    , window_length_ms_(window_length_ms)
    , step_ms_(step)
{
    window_length_samples_ = window_length_ms_ * soundfile_->getSampleRate();
    buffer_ = std::unique_ptr< CircularBuffer<float> >( new CircularBuffer<float>(window_length_samples_));
    nfft_ = next_pow_2(window_length_samples_);
    fft_config_ = kiss_fftr_alloc(nfft_, 0, nullptr, nullptr);
    fft_input_ = std::unique_ptr< std::vector<float> >(new std::vector<float>(nfft_));
    std::fill(fft_input_->begin(), fft_input_->end(), 0);
    assert(fft_config_ != nullptr);
}

PSDFeature::~PSDFeature() {
    kiss_fftr_free(fft_config_);
}


MatrixXf PSDFeature::descriptors()
{
    size_t step_in_sample = size_t(step_ms_ * soundfile_->getSampleRate());
    size_t n_frames = (size_t)soundfile_->getNumberOfSamples() / step_in_sample;

    MatrixXf desc(nfft_ / 2 + 1, n_frames);
    std::vector<kiss_fft_cpx> frequencies(nfft_ / 2 + 1);
    float pcm_buffer[step_in_sample];

    size_t read = 0;
    for (size_t frame_index = 0; frame_index < n_frames; frame_index++)
    {
        read = soundfile_->getFrame(step_in_sample, pcm_buffer);
        buffer_->append(pcm_buffer, read);
        memcpy(fft_input_->data(), buffer_->data(), buffer_->sizeInBytes());
        kiss_fftr(fft_config_, fft_input_->data(), frequencies.data());

        float *psd = desc.col(frame_index).data();
        for (auto& coef : frequencies) {
            (*psd++) = coef.i * coef.i + coef.r * coef.r;
        }
    }

    return desc;
}
