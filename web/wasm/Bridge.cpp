// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8472_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8636_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8636_Validation.hpp"
#include <emscripten/bind.h>
#include <algorithm>
#include <stdexcept>

namespace {
    namespace S = TransceiverTool::Standards;
    using Json = nlohmann::json;

    std::vector<unsigned char> bytes(const Json& value, std::size_t length) {
        if(!value.is_array() || value.size() != length) {
            throw std::invalid_argument("Unexpected binary length");
        }
        std::vector<unsigned char> result;
        for(const auto& byte : value) {
            if(!byte.is_number_integer() || byte < 0 || byte > 255) {
                throw std::invalid_argument("Binary values must be integers from 0 to 255");
            }
            result.push_back(byte.get<unsigned char>());
        }
        return result;
    }

    std::string request(const std::string& input) {
        try {
            if(input.size() > 1024 * 1024) {
                throw std::invalid_argument("Configuration exceeds the 1 MiB input limit");
            }
            const auto args = Json::parse(input);
            const auto standard = args.at("standard").get<std::string>();
            if(standard != "8472" && standard != "8636") {
                throw std::invalid_argument("Unsupported standard");
            }
            const auto operation = args.at("operation").get<std::string>();
            if(operation != "decode" && operation != "encode") {
                throw std::invalid_argument("Unsupported operation");
            }
            const bool fiber = args.value("fiber", true);
            const bool sfp = standard == "8472";
            std::vector<unsigned char> binary(sfp ? 128 : 256, 0);
            nlohmann::ordered_json document;
            S::common::ValidationResult validation;
            const auto checksum = S::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING;
            if(operation == "decode") {
                const auto& raw = args.at("bytes");
                if(sfp) {
                    binary = bytes(raw, 128);
                } else if(raw.size() == 128) {
                    const auto upper = bytes(raw, 128);
                    std::copy(upper.begin(), upper.end(), binary.begin() + 128);
                } else {
                    binary = bytes(raw, 256);
                }
            } else if(!sfp && args.contains("lowerBytes")) {
                const auto lower = bytes(args.at("lowerBytes"), 128);
                std::copy(lower.begin(), lower.end(), binary.begin());
            }
            if(sfp) {
                S::SFF8472::SFF8472_LowerA0h value;
                if(operation == "decode") {
                    value = S::SFF8472::parseBytesToStruct(binary.data());
                } else {
                    S::SFF8472::SFF8472_LowerA0hFromJSON(args.at("document"), value);
                    S::SFF8472::assembleToBinary(binary.data(), value, checksum, checksum);
                }
                S::SFF8472::SFF8472_LowerA0hToJSON(document, value, fiber);
                validation = S::SFF8472::Validation::validateSFF8472_LowerA0h(value);
            } else {
                S::SFF8636::SFF8636_Upper00h value;
                if(operation == "decode") {
                    value = S::SFF8636::parseBytesToStruct(binary.data());
                } else {
                    S::SFF8636::SFF8636_Upper00hFromJSON(args.at("document"), value);
                    S::SFF8636::assembleToBinary(binary.data(), value, checksum, checksum);
                }
                S::SFF8636::SFF8636_Upper00hToJSON(document, value, fiber);
                validation = S::SFF8636::Validation::validateSFF8636_Upper00h(value);
            }
            return nlohmann::ordered_json{
                {"ok", true},
                {"document", document},
                {"bytes", binary},
                {"errors", validation.errors},
                {"warnings", validation.warnings}
            }.dump();
        } catch(const std::exception& error) {
            return Json{{"ok", false}, {"error", error.what()}}.dump();
        }
    }
}  // namespace

EMSCRIPTEN_BINDINGS(transceiver_tool) {
    emscripten::function("request", &request);
}
