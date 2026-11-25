#pragma once

#include <cstdint>
#include <vector>

#include "audio_data.h"

bool convertToMono(AudioData& audio, void (*progressCallback)(int) = nullptr);
bool convertTo8Bit(const AudioData& audio, std::vector<int8_t>& output);
bool resampleAudio(AudioData& audio, int targetSampleRate, void (*progressCallback)(int) = nullptr);
