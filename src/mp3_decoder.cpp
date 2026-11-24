#define MINIMP3_IMPLEMENTATION
#include <string>
#include "minimp3_ex.h"
#include "mp3_decoder.h"

bool decodeMp3(const std::string& filename, AudioData& audio) {
    mp3dec_t decoder{};
    mp3dec_file_info_t info{};

    int result = mp3dec_load(&decoder, filename.c_str(), &info, nullptr, nullptr);

    if (result != 0) {
        free(info.buffer);
        return false;
    }

    audio.samples.assign(info.buffer, info.buffer + info.samples);
    audio.sampleRate = info.hz;
    audio.channels = info.channels;

    free(info.buffer);

    return true;
}