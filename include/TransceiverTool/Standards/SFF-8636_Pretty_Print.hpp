// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {
    std::string prettyPrintProgramming(const SFF8636_Upper00h& programming, bool fiberMode);
}