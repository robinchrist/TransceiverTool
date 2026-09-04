// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    // Expects full page 00h with 256 bytes!
    unsigned char calculateCC_BASEChecksum(unsigned char const* bytes);

    // Expects full page 00h with 256 bytes!
    unsigned char calculateCC_EXTChecksum(unsigned char const* bytes);
}  // namespace TransceiverTool::Standards::SFF8636