#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <vector>

#include "dfpwm_encoder.h"
#include "mp3_decoder.h"
#include "audio_processor.h"
using namespace std;

void reportMonoProgress(int percentage) {
    cout << "\r[2/4] Converting to Mono [" << percentage << "%]" << flush;

    if (percentage == 100) {
        cout << '\n';
    }
}

void reportResampleProgress(int percentage) {
    cout << "\r[3/4] Resampling Audio  [" << percentage << "%]" << flush;

    if (percentage == 100) {
        cout << '\n';
    }
}

void reportEncodeProgress(int percentage) {
    cout << "\r[4/4] Encoding DFPWM    [" << percentage << "%]" << flush;

    if (percentage == 100) {
        cout << '\n';
    }
}

string formatNumber(size_t number) {
    string result = to_string(number);

    for (size_t position = result.length(); position > 3; position -= 3) {
        result.insert(position - 3, ",");
    }

    return result;
}

int main(int argc, char* argv[]) {

    if (argc != 2 && argc != 4) {
        cerr << "Usage: dfpwm <input_path.mp3> -o <output_path>\n";
        return 1;
    }

    string inputFile = argv[1];
    string outputFileName;

    if (argc == 4) {
        if (string(argv[2]) != "-o") {
            cerr << "Unknown argument: " << argv[2] << '\n';
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
    string errorMessage;

    size_t lastSeparator = inputFile.find_last_of("/\\");
    string inputFileName = lastSeparator == string::npos ? inputFile : inputFile.substr(lastSeparator + 1);

    cout << "Input: " << inputFileName << '\n';

    if (!decodeMp3(inputFile, audio, errorMessage)) {
        cerr << "Error: " << errorMessage << '\n';
        return 1;
    }

    double duration = static_cast<double>(audio.samples.size()) / (static_cast<double>(audio.sampleRate) * audio.channels);
    unsigned long long totalSeconds = static_cast<unsigned long long>(duration);

    cout << "\nAudio: " << setfill('0') << setw(2) << totalSeconds / 60 << ":" << setw(2) << totalSeconds % 60 << setfill(' ');
    cout << " | " << audio.sampleRate << " Hz";
    cout << " | " << (audio.channels == 1 ? "Mono" : "Stereo");
    cout << " | 16-bit PCM\n\n";

    if (!convertToMono(audio, reportMonoProgress)) {
        cerr << "Audio processing failed!\n";
        return 1;
    }

    if (!resampleAudio(audio, 48000, reportResampleProgress)) {
        cerr << "Audio resampling failed!\n";
        return 1;
    }

    vector<int8_t> samples8Bit;

    if (!convertTo8Bit(audio, samples8Bit)) {
        cerr << "8-bit conversion failed!\n";
        return 1;
    }

    DfpwmEncoder encoder;
    vector<uint8_t> encodedAudio = encoder.encode(samples8Bit, reportEncodeProgress);

    ofstream outputFile(outputFileName, ios::binary);

    if (!outputFile) {
        cerr << "Could not create output file: " << outputFileName << '\n';
        return 1;
    }

    outputFile.write(reinterpret_cast<const char*>(encodedAudio.data()), static_cast<streamsize>(encodedAudio.size()));

    if (!outputFile) {
        cerr << "Failed to write DFPWM data!\n";
        return 1;
    }

    outputFile.close();

    if (!outputFile) {
        cerr << "Failed to close output file: " << outputFileName << '\n';
        return 1;
    }

    cout << "\nConversion successful!\n";
    cout << "Output: " << outputFileName << '\n';
    cout << "Size: " << formatNumber(encodedAudio.size()) << " bytes\n";

    return 0;
}
