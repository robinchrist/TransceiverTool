#pragma once
#include <string>
#include <array>

namespace TransceiverTool {

    struct VendorOUI {
        std::array<unsigned char, 3> byte_value;
        std::string name;
    };

    //Important / commonly encountered OUIs
    inline const std::array<VendorOUI, 16> VendorOUIs {{
        {{0x64, 0x9d, 0x99}, "FS COM INC"},
        {{0x00, 0x05, 0x1e}, "Brocade Communications Systems LLC"},
        {{0x00, 0x90, 0x65}, "FINISAR CORPORATION"},
        {{0x00, 0x1b, 0x21}, "Intel Corporate"},
        {{0x00, 0x02, 0xc9}, "Mellanox Technologies, Inc."},
        {{0x00, 0x17, 0x6a}, "Avago Technologies"},
        {{0x00, 0x00, 0x5f}, "Sumitomo Electric Industries, Ltd"},
        {{0x00, 0x40, 0x20}, "CommScope Inc"},
        {{0x00, 0x09, 0x3a}, "Molex CMS"},
        {{0x00, 0x14, 0x6e}, "IBM (IEEE is H. Stoll GmbH & Co. KG)"},
        {{0x00, 0x1c, 0x73}, "Arista Networks"},
        {{0x00, 0x1e, 0x62}, "Extreme (IEEE is Siemon)"},
        {{0x00, 0x25, 0xcd}, "Skylane Optics"},
        {{0x78, 0xa7, 0x14}, "Amphenol"},
        {{0x00, 0x14, 0x4f}, "Oraclel"},
        {{0x44, 0x7c, 0x7f}, "Innolight Technology Corporation"},
    }};
}