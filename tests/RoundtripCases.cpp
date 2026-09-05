// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#include <array>
#include <cstdint>
#include <cstddef>
#include <random>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, std::size_t size);

int main() {
    std::array<uint8_t, 256> bytes{};
    // Every byte value in every position exercises assigned names, reserved values, flags,
    // numeric endpoints, and raw string fallbacks. Reuse the exact fuzz target assertions.
    for(unsigned value = 0; value < 256; ++value) {
        bytes.fill(static_cast<uint8_t>(value));
        LLVMFuzzerTestOneInput(bytes.data(), bytes.size());
    }
    std::mt19937 random(0x84728636);
    for(unsigned sample = 0; sample < 128; ++sample) {
        for(auto& byte : bytes) {
            byte = static_cast<uint8_t>(random());
        }
        LLVMFuzzerTestOneInput(bytes.data(), bytes.size());
    }
}
