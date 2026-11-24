#pragma once

#include <cstdint>
#include <vector>

class DfpwmEncoder {
public:
    std::vector<uint8_t> encode(const std::vector<int8_t>& samples);

private:
    int predict(bool currentBit);
    int charge = 0;
    int strength = 0;
    bool previousBit = false;
    int previousCharge = 0;
};