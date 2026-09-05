// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#include "SchemaContract.hpp"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <nlohmann/json-schema.hpp>
#include <cppcodec/base64_rfc4648.hpp>

namespace {
    auto load(const char* filename) {
        std::ifstream input(std::string(TRANSCEIVERTOOL_SCHEMA_DIR) + "/" + filename);
        if(!input) {
            throw std::runtime_error(std::string("Cannot open schema: ") + filename);
        }
        auto validator = std::make_unique<nlohmann::json_schema::json_validator>(
            nullptr,
            nlohmann::json_schema::default_string_format_check,
            [](const std::string& encoding, const std::string& mediaType, const nlohmann::json& value) {
                if(encoding != "base64" || !mediaType.empty()) {
                    throw std::invalid_argument("Unsupported schema content encoding/media type");
                }
                (void)cppcodec::base64_rfc4648::decode(value.get<std::string>());
            }
        );
        validator->set_root_schema(nlohmann::json::parse(input));
        return validator;
    }
}  // namespace

namespace SchemaContract {
    void validate8472(const nlohmann::json& document) {
        static const auto validator = load("SFF-8472_LowerA0h.json");
        validator->validate(document);
    }
    void validate8636(const nlohmann::json& document) {
        static const auto validator = load("SFF-8636_Upper00h.json");
        validator->validate(document);
    }
}  // namespace SchemaContract
