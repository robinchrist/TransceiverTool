// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8472 {

    // Expects 128 bytes!
    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h parseBytesToStruct(unsigned char const* bytes);

}  // namespace TransceiverTool::Standards::SFF8472