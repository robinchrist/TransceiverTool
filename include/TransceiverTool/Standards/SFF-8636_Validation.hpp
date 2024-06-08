#pragma once
#include <string>
#include <array>
#include <vector>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {

    struct ValidationResult {
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
    };

    ValidationResult validateSFF8636_Upper00h(const SFF8636_Upper00h& programming);
}