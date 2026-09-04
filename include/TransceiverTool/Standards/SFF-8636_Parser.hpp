// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8636 {

    // Expects 256 bytes!
    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h parseBytesToStruct(unsigned char const* bytes);

}  // namespace TransceiverTool::Standards::SFF8636