#include "audio_processor.h"
#include <utility>
#include <cmath>
#include <algorithm>

bool convertToMono(AudioData& audio) {

    if (audio.channels == 1) {
        return true;
    }

    if (audio.channels != 2) {
        return false;
    }

    std::vector<int16_t> monoSamples;

    if (audio.samples.size() % 2 != 0) {
        return false;
    }

    monoSamples.reserve(audio.samples.size() / 2);

    for (size_t i = 0; i < audio.samples.size(); i += 2) {
        int16_t left = audio.samples[i];
        int16_t right = audio.samples[i + 1];
        int32_t mono = (static_cast<int32_t>(left) + right) / 2;
        monoSamples.push_back(static_cast<int16_t>(mono));
    }

    audio.samples = std::move(monoSamples);
    audio.channels = 1;

    return true;
}

bool resampleAudio(AudioData& audio, int targetSampleRate) {
    constexpr double pi = 3.14159265358979323846;
    if (audio.sampleRate <= 0 || targetSampleRate <= 0 || audio.channels <= 0 || audio.samples.empty()) {
        return false;
    }

    if (audio.samples.size() % audio.channels != 0) {
        return false;
    }

    if (audio.sampleRate == targetSampleRate) {
        return true;
    }

    const size_t inputFrames = audio.samples.size() / audio.channels;
    const double ratio = static_cast<double>(targetSampleRate) / audio.sampleRate;
    const double cutoff = std::min(1.0, ratio);
    const double filterRadius = 16.0 / cutoff;

    const double outputFrameCount = std::round(inputFrames * ratio);

    std::vector<int16_t> resampledSamples;

    if (outputFrameCount < 1 || outputFrameCount > resampledSamples.max_size() / audio.channels) {
        return false;
    }

    const size_t outputFrames = static_cast<size_t>(outputFrameCount);
    resampledSamples.resize(outputFrames * audio.channels);

    for (size_t outputFrame = 0; outputFrame < outputFrames; ++outputFrame) {

        const double sourcePosition = outputFrame / ratio;
        const long long center = static_cast<long long>(std::floor(sourcePosition));

        const long long radius = static_cast<long long>(std::ceil(filterRadius));
        const long long firstSample = std::max(0LL, center - radius);
        const long long lastSample = std::min(static_cast<long long>(inputFrames) - 1, center + radius);

        for (int channel = 0; channel < audio.channels; ++channel) {

            double weightedSum = 0.0;
            double weightSum = 0.0;

            for (long long inputFrame = firstSample; inputFrame <= lastSample; ++inputFrame) {

                const double distance = sourcePosition - inputFrame;

                if (std::abs(distance) >= filterRadius) {
                    continue;
                }

                const double sincPosition = distance * cutoff;
                const double sinc = sincPosition == 0.0 ? 1.0 : std::sin(pi * sincPosition) / (pi * sincPosition);
                const double window = 0.5 * (1.0 + std::cos(pi * distance / filterRadius));
                const double weight = cutoff * sinc * window;
                const size_t inputIndex = static_cast<size_t>(inputFrame) * audio.channels + channel;
                weightedSum += audio.samples[inputIndex] * weight;
                weightSum += weight;
            }

            if (std::abs(weightSum) < 1e-12) {
                return false;
            }

            const double sample = weightedSum / weightSum;
            const size_t outputIndex = outputFrame * audio.channels + channel;

            resampledSamples[outputIndex] = static_cast<int16_t>(std::clamp(std::lround(sample), -32768L, 32767L));
        }
    }

    audio.samples = std::move(resampledSamples);
    audio.sampleRate = targetSampleRate;

    return true;
}

bool convertTo8Bit(const AudioData& audio, std::vector<int8_t>& output) {
    output.clear();
    output.reserve(audio.samples.size());

    for (int16_t sample : audio.samples) {
        int8_t sample8 = static_cast<int8_t>(sample / 256);
        output.push_back(sample8);
    }

    return true;
}

