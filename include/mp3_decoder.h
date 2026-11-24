#pragma once
#include <string>
#include <cstdint>
#include <vector>

struct AudioData {
    std::vector<int16_t> samples;
    int sampleRate;
    int channels;
};
bool decodeMp3(const std::string& filename, AudioData& audio);