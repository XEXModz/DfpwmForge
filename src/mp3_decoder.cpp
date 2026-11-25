#define MINIMP3_IMPLEMENTATION
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <cstdlib>
#include "minimp3_ex.h"
#include "mp3_decoder.h"

bool decodeMp3(const std::string& filename, AudioData& audio, std::string& errorMessage) {
    
    std::string extension = filename.substr(filename.find_last_of('.') + 1);

    std::transform(extension.begin(), extension.end(), extension.begin(), [] (unsigned char character) {return static_cast<char>(std::tolower(character));});

    if (extension != "mp3") {
        errorMessage = "Unsupported input file format. Only .mp3 files are supported.";
        return false;
    }
    
    std::ifstream inputFile(filename, std::ios::binary | std::ios::ate);

    if (!inputFile) {
        errorMessage = "Could not open input file: " + filename;
        return false;
    }

    std::streampos fileSize = inputFile.tellg();

    if (fileSize == std::streampos(-1)) {
        errorMessage = "Could not determine input file size: " + filename;
        return false;
    }

    if (fileSize == 0) {
        errorMessage = "Input file is empty: " + filename;
        return false;
    }

    inputFile.close();
    mp3dec_t decoder{};
    mp3dec_file_info_t info{};

    int result = mp3dec_load(&decoder, filename.c_str(), &info, nullptr, nullptr);

    if (result != 0) {
        free(info.buffer);
        errorMessage = "Failed to decode MP3 file: " + filename;
        return false;
    }

    if (info.buffer == nullptr || info.samples == 0) {
        free(info.buffer);
        errorMessage = "MP3 file contains no audio samples.";
        return false;
    }

    if (info.hz <= 0) {
        free(info.buffer);
        errorMessage = "MP3 file has an invalid sample rate.";
        return false;
    }

    if (info.channels != 1 && info.channels != 2) {
        free(info.buffer);
        errorMessage = "Unsupported audio channel count: " + std::to_string(info.channels);
        return false;
    }

    audio.samples.assign(info.buffer, info.buffer + info.samples);
    audio.sampleRate = info.hz;
    audio.channels = info.channels;

    free(info.buffer);

    errorMessage.clear();
    return true;
}
