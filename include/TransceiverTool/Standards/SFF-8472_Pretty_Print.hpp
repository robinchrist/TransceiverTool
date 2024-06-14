#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"

namespace TransceiverTool::Standards::SFF8472 {
    std::string prettyPrintProgramming(const SFF8472_LowerA0h& programming, bool fiberMode, bool copperMode);
}