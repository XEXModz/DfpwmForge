#define MINIMP3_IMPLEMENTATION
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <cstdlib>
#include "minimp3_ex.h"
#include "mp3_decoder.h"
using namespace std;

struct DecodeProgress {
    int lastPercentage = -1;
};

int reportDecodeProgress(void* userData, size_t fileSize, uint64_t offset, mp3dec_frame_info_t*) {
    auto* progress = static_cast<DecodeProgress*>(userData);

    if (fileSize == 0) {
        return 0;
    }

    int percentage = static_cast<int>((static_cast<double>(offset) / fileSize) * 100.0);

    if (percentage > 100) {
        percentage = 100;
    }

    if (percentage != progress->lastPercentage) {
        cout << "\r[1/4] Decoding MP3       [" << percentage << "%]" << flush;
        progress->lastPercentage = percentage;
    }

    return 0;
}

bool decodeMp3(const string& filename, AudioData& audio, string& errorMessage) {
    
    string extension = filename.substr(filename.find_last_of('.') + 1);

    transform(extension.begin(), extension.end(), extension.begin(), [] (unsigned char character) {return static_cast<char>(tolower(character));});

    if (extension != "mp3") {
        errorMessage = "Unsupported input file format. Only .mp3 files are supported.";
        return false;
    }
    
    ifstream inputFile(filename, ios::binary | ios::ate);

    if (!inputFile) {
        errorMessage = "Could not open input file: " + filename;
        return false;
    }

    streampos fileSize = inputFile.tellg();

    if (fileSize == streampos(-1)) {
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
    DecodeProgress progress;

    int result = mp3dec_load(&decoder, filename.c_str(), &info, reportDecodeProgress, &progress);

    if (result != 0) {
        cout << '\n';
        free(info.buffer);
        errorMessage = "Failed to decode MP3 file: " + filename;
        return false;
    }

    cout << "\r[1/4] Decoding MP3       [100%]\n";

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
        errorMessage = "Unsupported audio channel count: " + to_string(info.channels);
        return false;
    }

    audio.samples.assign(info.buffer, info.buffer + info.samples);
    audio.sampleRate = info.hz;
    audio.channels = info.channels;

    free(info.buffer);

    errorMessage.clear();
    return true;
}
