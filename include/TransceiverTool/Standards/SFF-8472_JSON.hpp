#pragma once
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"

namespace TransceiverTool::Standards::SFF8472 {

    void SFF8472_LowerA0hToJSON(nlohmann::ordered_json& j, const SFF8472_LowerA0h& programming, bool fiberMode);
    void SFF8472_LowerA0hFromJSON(const nlohmann::json& j, SFF8472_LowerA0h& programming);
    
}