#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {
    std::string prettyPrintProgramming(const SFF8636_Upper00h& programming, bool fiberMode);
}