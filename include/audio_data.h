#pragma once

#include <cstdint>
#include <vector>

struct AudioData {
    std::vector<int16_t> samples;
    int sampleRate;
    int channels;
};
