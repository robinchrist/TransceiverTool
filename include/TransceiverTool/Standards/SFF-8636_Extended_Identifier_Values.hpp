#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129) bits 7-6
    enum class Extended_Identifier_Bit_7_6 {
        POWER_CLASS_1_0b00,
        POWER_CLASS_2_0b01,
        POWER_CLASS_3_0b10,
        POWER_CLASS_4_TO_7_0b11
    };
    struct Extended_Identifier_Bit_7_6_string {
        Extended_Identifier_Bit_7_6 enum_value;
        std::string description;
        unsigned char bitValue;
    };
    inline const std::array<Extended_Identifier_Bit_7_6_string, 4> Extended_Identifier_Bit_7_6_strings {{
        {Extended_Identifier_Bit_7_6::POWER_CLASS_1_0b00, "Power Class 1 (1.5 W max.)", 0b00},
        {Extended_Identifier_Bit_7_6::POWER_CLASS_2_0b01, "Power Class 2 (2.0 W max.)", 0b01},
        {Extended_Identifier_Bit_7_6::POWER_CLASS_3_0b10, "Power Class 3 (2.5 W max.)", 0b10},
        {Extended_Identifier_Bit_7_6::POWER_CLASS_4_TO_7_0b11, "Power Class 4 (3.5 W max.) and Power Classes 5, 6 or 7", 0b11},
    }};

    const Extended_Identifier_Bit_7_6_string& getSFF8636_Extended_Identifier_Bit_7_6Info(Extended_Identifier_Bit_7_6 enum_value);
    Extended_Identifier_Bit_7_6 getSFF8636_Extended_Identifier_Bit_7_6_From_Char(unsigned char value);

    //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129) bits 1-0
    enum class Extended_Identifier_Bit_1_0 {
        POWER_CLASSES_1_TO_4_0b00,
        POWER_CLASS_5_0b01,
        POWER_CLASS_6_0b10,
        POWER_CLASS_7_0b11,
    };
    struct Extended_Identifier_Bit_1_0_string {
        Extended_Identifier_Bit_1_0 enum_value;
        std::string description;
        unsigned char bitValue;
    };
    inline const std::array<Extended_Identifier_Bit_1_0_string, 4> Extended_Identifier_Bit_1_0_strings {{
        {Extended_Identifier_Bit_1_0::POWER_CLASSES_1_TO_4_0b00, "Power Classes 1 to 4", 0b00},
        {Extended_Identifier_Bit_1_0::POWER_CLASS_5_0b01, "Power Class 5 (4.0 W max.)", 0b01},
        {Extended_Identifier_Bit_1_0::POWER_CLASS_6_0b10, "Power Class 6 (4.5 W max.)", 0b10},
        {Extended_Identifier_Bit_1_0::POWER_CLASS_7_0b11, "Power Class 7 (5.0 W max.)", 0b11},
    }};
    const Extended_Identifier_Bit_1_0_string& getSFF8636_Extended_Identifier_Bit_1_0Info(Extended_Identifier_Bit_1_0 enum_value);
    Extended_Identifier_Bit_1_0 getSFF8636_Extended_Identifier_Bit_1_0_From_Char(unsigned char value);

    struct Extended_Identifier {
        //Power Class Related Bit 7 & 6
        Extended_Identifier_Bit_7_6 power_class_bit_7_6;

        //Power Class 8 implemented (Max power declared in byte 107)
        bool power_class_8_bit_5;

        //CLEI code present in Page 02h (0 = Not Present, 1 = Present)
        bool clei_code_bit_4;

        //CDR present in Tx (0 = No CDR in Tx, 1 = CDR present in Tx)
        bool cdr_in_tx_bit_3;

        //CDR present in Tx (0 = No CDR in Rx, 1 = CDR present in Rx)
        bool cdr_in_rx_bit_2;

        //Power Class Related Bit 1 & 0
        Extended_Identifier_Bit_1_0 power_class_bit_1_0;
    };
}