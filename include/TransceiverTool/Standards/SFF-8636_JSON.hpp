#pragma once
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {

    void SFF8636_Upper00hToJSON(nlohmann::ordered_json& j, const SFF8636_Upper00h& programming, bool fiberMode);
    void SFF8636_Upper00hFromJSON(const nlohmann::json& j, SFF8636_Upper00h& programming);
    
}