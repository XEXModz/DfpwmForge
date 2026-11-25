#include "dfpwm_encoder.h"
using namespace std;

namespace {
    constexpr int PRECISION = 10;
    constexpr int PRECISION_SCALE = 1 << PRECISION;       // 1024
    constexpr int PRECISION_HALF = PRECISION_SCALE / 2;   // 512
    constexpr int MIN_STRENGTH = 8;

    int floorDiv1024(int value) {
        if (value >= 0) {
            return value / PRECISION_SCALE;
        }

        return -((-value + PRECISION_SCALE - 1) / PRECISION_SCALE);
    }
}

int DfpwmEncoder::predict(bool currentBit) {
    int target = currentBit ? 127 : -128;

    int nextCharge = charge + floorDiv1024(strength * (target - charge) + PRECISION_HALF);

    if (nextCharge == charge && nextCharge != target) {
        nextCharge += currentBit ? 1 : -1;
    }

    bool sameBit = currentBit == previousBit;

    int targetStrength = sameBit ? PRECISION_SCALE - 1 : 0;
    int nextStrength = strength;

    if (nextStrength != targetStrength) {
        nextStrength += sameBit ? 1 : -1;
    }

    if (nextStrength < MIN_STRENGTH) {
        nextStrength = MIN_STRENGTH;
    }

    charge = nextCharge;
    strength = nextStrength;
    previousBit = currentBit;

    return charge;
}

vector<uint8_t> DfpwmEncoder::encode(const vector<int8_t>& samples, void (*progressCallback)(int)) {

    vector<uint8_t> output;

    // Each output byte stores 8 audio samples.
    output.reserve((samples.size() + 7) / 8);

    int lastReportedPercentage = -1;

    for (size_t i = 0; i < samples.size(); i += 8) {
        uint8_t thisByte = 0;

        for (size_t j = 0; j < 8; j++) {
            size_t index = i + j;

            // Pad the final byte with silence if fewer than 8 samples remain.
            int inputCharge = (index < samples.size()) ? samples[index] : 0;

            bool currentBit = inputCharge > previousCharge || (inputCharge == previousCharge && inputCharge == 127);

            // Packs the bits in CC:Tweaked's expected bit order.
            thisByte = static_cast<uint8_t>((thisByte >> 1) | (currentBit ? 0x80 : 0));

            previousCharge = predict(currentBit);
        }

        output.push_back(thisByte);

        if (progressCallback) {
            const size_t remainingSamples = samples.size() - i;
            const size_t processedSamples = i + (remainingSamples < 8 ? remainingSamples : 8);
            const int percentage = static_cast<int>((static_cast<double>(processedSamples) / samples.size()) * 100.0);

            if (percentage != lastReportedPercentage) {
                progressCallback(percentage);
                lastReportedPercentage = percentage;
            }
        }
    }

    return output;
}
