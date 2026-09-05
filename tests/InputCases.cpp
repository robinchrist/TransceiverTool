// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#include "SchemaContract.hpp"
#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8636_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8636_Parser.hpp"
#include <array>
#include <iostream>
#include <stdexcept>

namespace {
    using Json = nlohmann::json;

    template <typename Function>
    bool accepts(Function function, const Json& document) {
        try {
            function(document);
            return true;
        } catch(const std::exception&) {
            return false;
        }
    }

    template <typename Validate, typename Parse>
    void cases(const Json& baseline, Validate validate, Parse parse) {
        // Validate the baseline outside the rejection checks: schema loading errors must fail.
        validate(baseline);
        parse(baseline);
        auto check = [&](const char* label, Json document, bool expected) {
            // Exercise text input, including nlohmann's signed/unsigned number classification.
            document = Json::parse(document.dump());
            if(accepts(validate, document) != expected || accepts(parse, document) != expected) {
                throw std::runtime_error(std::string("Schema/parser contract mismatch: ") + label);
            }
        };
        auto field = [&](const char* key, const Json& value, bool expected) {
            auto document = baseline;
            document[key] = value;
            check(key, document, expected);
        };

        field("Identifier", "Unknown or unspecified", true);
        field("Identifier", Json{{"byteValue", "0xff"}}, true);
        field("Identifier", Json{{"byteValue", "0xAF"}}, true);
        field("Identifier", Json{{"byteValue", "0x100"}}, false);
        field("Identifier", Json{{"byteValue", "0xGG"}}, false);
        field("Identifier", Json::object(), false);
        field("Identifier", "not an assigned identifier", false);
        field("Identifier", nullptr, false);
        field("CC_BASE", "auto", true);
        field("CC_BASE", Json{{"byteValue", "0x00"}}, true);
        field("CC_BASE", "automatic", false);
        field("Vendor Name", "", true);
        field("Vendor Name", "0123456789abcdef", true);
        field("Vendor Name", "0123456789abcdefg", false);
        field("Vendor Name", Json{{"Type", "Base64"}, {"Value", "AAAAAAAAAAAAAAAAAAAAAA=="}}, true);
        field("Type", "wrong standard", false);
        for(const auto value : {0, 100, 200, 25400}) {
            field("Nominal Signaling Rate [MBaud] (Divisible by 100)", value, true);
        }
        for(const auto value : {-100, 105, 25500}) {
            field("Nominal Signaling Rate [MBaud] (Divisible by 100)", value, false);
        }
        auto document = baseline;
        document.erase("Identifier");
        check("missing Identifier", document, false);
        document = baseline;
        document.erase("GeneratedBy");
        check("optional GeneratedBy", document, true);
    }
}  // namespace

int main() {
    namespace S = TransceiverTool::Standards;
    try {
        std::array<unsigned char, 256> bytes{};
        nlohmann::ordered_json a, b;
        S::SFF8472::SFF8472_LowerA0hToJSON(a, S::SFF8472::parseBytesToStruct(bytes.data()), true);
        S::SFF8636::SFF8636_Upper00hToJSON(b, S::SFF8636::parseBytesToStruct(bytes.data()), true);
        cases(Json::parse(a.dump()), SchemaContract::validate8472, [](const Json& document) {
            S::SFF8472::SFF8472_LowerA0h value;
            S::SFF8472::SFF8472_LowerA0hFromJSON(document, value);
        });
        cases(Json::parse(b.dump()), SchemaContract::validate8636, [](const Json& document) {
            S::SFF8636::SFF8636_Upper00h value;
            S::SFF8636::SFF8636_Upper00hFromJSON(document, value);
        });
    } catch(const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
