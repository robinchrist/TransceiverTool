#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {
    //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
    enum class Extended_Rate_Select_Compliance_Bit_1_0 {
        RESERVED_0b00,
        RATE_SELECT_VERSION_1_0b01,
        RATE_SELECT_VERSION_2_0b10,
        RESERVED_0b11
    };
    struct Extended_Rate_Select_Compliance_Bit_1_0_string {
        Extended_Rate_Select_Compliance_Bit_1_0 enum_value;
        std::string description;
        unsigned char bitValue;
    };
    inline const std::array<Extended_Rate_Select_Compliance_Bit_1_0_string, 4> Extended_Rate_Select_Compliance_Bit_1_0_strings {{
        {Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b00, "Reserved (00b)", 0b00},
        {Extended_Rate_Select_Compliance_Bit_1_0::RATE_SELECT_VERSION_1_0b01, "Rate Select Version 1", 0b01},
        {Extended_Rate_Select_Compliance_Bit_1_0::RATE_SELECT_VERSION_2_0b10, "Rate Select Version 2", 0b10},
        {Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b11, "Reserved (11b)", 0b11},
    }};
    const Extended_Rate_Select_Compliance_Bit_1_0_string& getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(Extended_Rate_Select_Compliance_Bit_1_0 enum_value);

    struct Extended_Rate_Select_Compliance {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool reserved_bit_5;
        bool reserved_bit_4;
        bool reserved_bit_3;
        bool reserved_bit_2;
        Extended_Rate_Select_Compliance_Bit_1_0 rate_select_bits_1_0;
    };
}