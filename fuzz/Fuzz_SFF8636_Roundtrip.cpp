// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

// libFuzzer target for the SFF-8636 binary -> JSON -> binary roundtrip.
//
// Same invariant as the SFF-8472 target: a programming must survive the JSON representation unchanged,
// and the tool must be able to read back the JSON it produced itself.
//
// SFF-8636 works on a full 256 byte page 00h, but only the upper half is described by SFF8636_Upper00h,
// so only bytes 128-255 are compared - the assembler does not write the lower half at all.
//
// See the fuzzing section in README.MD for how to build and run this.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <nlohmann/json.hpp>

#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8636_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace {

    // Full page 00h, the amount parseBytesToStruct expects
    constexpr std::size_t PAGE_SIZE = 256;
    // Only the upper page is modelled, so the comparison starts here
    constexpr std::size_t UPPER_PAGE_OFFSET = 128;

    void roundtrip(unsigned char const* bytes, bool fiberMode) {
        const auto parsed = TransceiverTool::Standards::SFF8636::parseBytesToStruct(bytes);

        nlohmann::ordered_json j;
        TransceiverTool::Standards::SFF8636::SFF8636_Upper00hToJSON(j, parsed, fiberMode);

        // Dump and re-parse so the actual text representation is exercised, not just the DOM
        TransceiverTool::Standards::SFF8636::SFF8636_Upper00h roundtripped;
        TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(nlohmann::json::parse(j.dump()), roundtripped);

        std::vector<unsigned char> reassembled(PAGE_SIZE, 0x00);
        TransceiverTool::Standards::SFF8636::assembleToBinary(
            reassembled.data(),
            roundtripped,
            TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
            TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
        );

        if(std::memcmp(
               reassembled.data() + UPPER_PAGE_OFFSET, bytes + UPPER_PAGE_OFFSET, PAGE_SIZE - UPPER_PAGE_OFFSET
           ) == 0) {
            return;
        }

        for(std::size_t index = UPPER_PAGE_OFFSET; index < PAGE_SIZE; ++index) {
            if(reassembled[index] != bytes[index]) {
                std::fprintf(
                    stderr,
                    "SFF-8636 roundtrip mismatch (fiberMode = %s) at byte %zu: expected 0x%02x, got 0x%02x\n",
                    fiberMode ? "true" : "false",
                    index,
                    bytes[index],
                    reassembled[index]
                );
            }
        }
        std::fprintf(stderr, "Intermediate JSON was:\n%s\n", j.dump(4).c_str());
        std::abort();
    }

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, std::size_t size) {
    // Returning -1 keeps short inputs out of the corpus instead of wasting mutations on them
    if(size < PAGE_SIZE) {
        return -1;
    }

    roundtrip(reinterpret_cast<unsigned char const*>(data), true);
    roundtrip(reinterpret_cast<unsigned char const*>(data), false);

    return 0;
}
