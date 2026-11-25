#pragma once

#include <string>
#include "audio_data.h"

bool decodeMp3(const std::string& filename, AudioData& audio, std::string& errorMessage);
