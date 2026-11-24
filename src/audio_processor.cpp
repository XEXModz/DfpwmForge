#include "audio_processor.h"
#include <utility>

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

bool convertTo8Bit(const AudioData& audio, std::vector<int8_t>& output) {
    output.clear();
    output.reserve(audio.samples.size());

    for (int16_t sample : audio.samples) {
        int8_t sample8 = static_cast<int8_t>(sample / 256);
        output.push_back(sample8);
    }

    return true;
}

