#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "dfpwm_encoder.h"
#include "mp3_decoder.h"
#include "audio_processor.h"

int main(int argc, char* argv[]) {

    if (argc != 2 && argc != 4) {
        std::cerr << "Usage: dfpwm <input_path.mp3> -o <output_path>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFileName;

    if (argc == 4) {
        if (std::string(argv[2]) != "-o") {
            std::cerr << "Unknown argument: " << argv[2] << '\n';
            return 1;
        }

        outputFileName = argv[3];
    } else {
        outputFileName = inputFile.substr(0, inputFile.find_last_of('.')) + ".dfpwm";
    }

    if (outputFileName.size() < 6 || outputFileName.substr(outputFileName.size() - 6) != ".dfpwm") {
        outputFileName += ".dfpwm";
    }

    AudioData audio;

    if (!decodeMp3(inputFile, audio)) {
        std::cerr << "Failed to decode MP3.\n";
        return 1;
    }

    if (!convertToMono(audio)) {
        std::cerr << "Audio processing failed!\n";
        return 1;
    }

    std::vector<int8_t> samples8Bit;

    if (!convertTo8Bit(audio, samples8Bit)) {
        std::cerr << "8-bit conversion failed!\n";
        return 1;
    }

    DfpwmEncoder encoder;
    std::vector<uint8_t> encodedAudio = encoder.encode(samples8Bit);

    std::ofstream outputFile(outputFileName, std::ios::binary);

    if (!outputFile) {
        std::cerr << "Could not create output file: " << outputFileName << '\n';
        return 1;
    }

    outputFile.write( reinterpret_cast<const char*>(encodedAudio.data()), static_cast<std::streamsize>(encodedAudio.size()) );

    if (!outputFile) {
        std::cerr << "Failed to write DFPWM data!\n";
        return 1;
    }

    std::cout << "Conversion successful!\n";
    std::cout << "Input: " << inputFile << '\n';
    std::cout << "Output: " << outputFileName << '\n';
    std::cout << "Encoded bytes: " << encodedAudio.size() << '\n';

    return 0;
}
