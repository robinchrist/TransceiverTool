#pragma once
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {

    constexpr const char* SFF8636_JSON_TYPE = "SFF-8636 Rev 2.11 Upper Page 00h";

    void SFF8636_Upper00hToJSON(nlohmann::ordered_json& j, const SFF8636_Upper00h& programming, bool fiberMode);
    void SFF8636_Upper00hFromJSON(const nlohmann::json& j, SFF8636_Upper00h& programming);
    
}