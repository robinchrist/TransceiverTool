// libFuzzer target for the JSON input path, i.e. what "TransceiverTool encode" is exposed to.
//
// Unlike the roundtrip targets this one is fed arbitrary bytes rather than tool-generated JSON, so
// rejecting the input is the expected outcome almost every time. Anything thrown as a std::exception
// is therefore caught and ignored - the findings here are crashes, sanitizer reports, hangs, and
// exceptions that are not derived from std::exception (those would escape the CLI's handler and
// terminate instead of printing an error).
//
// Everything the CLI does with a parsed programming is run, since assembling, pretty printing and
// validating are all reachable from a JSON file the user supplies.
//
// A dictionary of the JSON keys is in json_input.dict - without it the mutator has no realistic chance
// of producing the long, literal key names this format uses.
//
// See the fuzzing section in README.MD for how to build and run this.

#include <cstddef>
#include <cstdint>
#include <exception>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8472_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8636_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/SFF-8636_Validation.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace {

    namespace common = TransceiverTool::Standards::common;
    namespace SFF8472 = TransceiverTool::Standards::SFF8472;
    namespace SFF8636 = TransceiverTool::Standards::SFF8636;

    void exerciseSFF8472(const nlohmann::json& parsed) {
        SFF8472::SFF8472_LowerA0h programming;
        SFF8472::SFF8472_LowerA0hFromJSON(parsed, programming);

        std::vector<unsigned char> binary(128, 0x00);
        SFF8472::assembleToBinary(
            binary.data(),
            programming,
            common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
            common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
        );

        // Both meanings of the overloaded bytes are reachable from the CLI
        (void)SFF8472::prettyPrintProgramming(programming, true);
        (void)SFF8472::prettyPrintProgramming(programming, false);
        (void)SFF8472::Validation::validateSFF8472_LowerA0h(programming);
    }

    void exerciseSFF8636(const nlohmann::json& parsed) {
        SFF8636::SFF8636_Upper00h programming;
        SFF8636::SFF8636_Upper00hFromJSON(parsed, programming);

        std::vector<unsigned char> binary(256, 0x00);
        SFF8636::assembleToBinary(
            binary.data(),
            programming,
            common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
            common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
        );

        (void)SFF8636::prettyPrintProgramming(programming, true);
        (void)SFF8636::prettyPrintProgramming(programming, false);
        (void)SFF8636::Validation::validateSFF8636_Upper00h(programming);
    }

}  // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, std::size_t size) {
    try {
        const nlohmann::json parsed = nlohmann::json::parse(data, data + size);
        const auto type = parsed.at("Type").template get<std::string>();

        if(type == SFF8472::SFF8472_JSON_TYPE) {
            exerciseSFF8472(parsed);
        } else if(type == SFF8636::SFF8636_JSON_TYPE) {
            exerciseSFF8636(parsed);
        }
    } catch(const std::exception&) {
        // Rejecting malformed input with an exception is the correct behaviour, not a finding.
        // Deliberately not catching (...) so that anything not derived from std::exception escapes
        // and is reported, because the CLI would not handle it either.
    }

    return 0;
}
