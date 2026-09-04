// libFuzzer target for the SFF-8472 binary -> JSON -> binary roundtrip.
//
// The invariant under test is the one the README promises: every 128 byte programming, valid or not,
// survives a trip through the JSON representation unchanged. A byte mismatch means the JSON layer lost
// or altered information. An exception escaping FromJSON is just as much a finding - it means the tool
// cannot read back JSON that it produced itself.
//
// Both fiber and copper interpretation are exercised, because several bytes have two meanings and only
// one of them is covered per run.
//
// See the fuzzing section in README.MD for how to build and run this.

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <nlohmann/json.hpp>

#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace {

    // SFF-8472 lower page A0h, the amount parseBytesToStruct expects
    constexpr std::size_t PROGRAMMING_SIZE = 128;

    void roundtrip(unsigned char const* bytes, bool fiberMode) {
        const auto parsed = TransceiverTool::Standards::SFF8472::parseBytesToStruct(bytes);

        nlohmann::ordered_json j;
        TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hToJSON(j, parsed, fiberMode);

        // Dump and re-parse so the actual text representation is exercised, not just the DOM
        TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h roundtripped;
        TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hFromJSON(nlohmann::json::parse(j.dump()), roundtripped);

        std::vector<unsigned char> reassembled(PROGRAMMING_SIZE, 0x00);
        TransceiverTool::Standards::SFF8472::assembleToBinary(
            reassembled.data(),
            roundtripped,
            TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
            TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
        );

        if(std::memcmp(reassembled.data(), bytes, PROGRAMMING_SIZE) == 0) {
            return;
        }

        for(std::size_t index = 0; index < PROGRAMMING_SIZE; ++index) {
            if(reassembled[index] != bytes[index]) {
                std::fprintf(
                    stderr,
                    "SFF-8472 roundtrip mismatch (fiberMode = %s) at byte %zu: expected 0x%02x, got 0x%02x\n",
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
    if(size < PROGRAMMING_SIZE) {
        return -1;
    }

    roundtrip(reinterpret_cast<unsigned char const*>(data), true);
    roundtrip(reinterpret_cast<unsigned char const*>(data), false);

    return 0;
}
