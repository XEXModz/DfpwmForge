#pragma once

#include "mp3_decoder.h"

bool convertToMono(AudioData& audio);
bool convertTo8Bit(const AudioData& audio, std::vector<int8_t>& output);