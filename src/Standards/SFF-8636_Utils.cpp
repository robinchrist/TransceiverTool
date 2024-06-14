#include <algorithm>
#include "TransceiverTool/Standards/SFF-8636_Device_And_Transmitter_Technology.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Rate_Select_Compliance.hpp"


namespace TransceiverTool::Standards::SFF8636 {
    const Extended_Identifier_Bit_7_6_string& getSFF8636_Extended_Identifier_Bit_7_6Info(Extended_Identifier_Bit_7_6 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_7_6_strings.begin(),
            Extended_Identifier_Bit_7_6_strings.end(),
            [enum_value](const Extended_Identifier_Bit_7_6_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    Extended_Identifier_Bit_7_6 getSFF8636_Extended_Identifier_Bit_7_6_From_Char(unsigned char value) {
        return std::find_if(
            Extended_Identifier_Bit_7_6_strings.begin(),
            Extended_Identifier_Bit_7_6_strings.end(),
            [value](const Extended_Identifier_Bit_7_6_string& entry) { return entry.bitValue == ((value >> 6) & 0b00000011 ); }
        )->enum_value;
    }


    const Extended_Identifier_Bit_1_0_string& getSFF8636_Extended_Identifier_Bit_1_0Info(Extended_Identifier_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_1_0_strings.begin(),
            Extended_Identifier_Bit_1_0_strings.end(),
            [enum_value](const Extended_Identifier_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    Extended_Identifier_Bit_1_0 getSFF8636_Extended_Identifier_Bit_1_0_From_Char(unsigned char value) {
        return std::find_if(
            Extended_Identifier_Bit_1_0_strings.begin(),
            Extended_Identifier_Bit_1_0_strings.end(),
            [value](const Extended_Identifier_Bit_1_0_string& entry) { return entry.bitValue == ((value >> 0) & 0b00000011 ); }
        )->enum_value;
    }


    const Extended_Rate_Select_Compliance_Bit_1_0_string& getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(Extended_Rate_Select_Compliance_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
            Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
            [enum_value](const Extended_Rate_Select_Compliance_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    Extended_Rate_Select_Compliance_Bit_1_0 getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0_From_Char(unsigned char value) {
        return std::find_if(
            Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
            Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
            [value](const Extended_Rate_Select_Compliance_Bit_1_0_string& entry) { return entry.bitValue == ((value >> 0) & 0b00000011 ); }
        )->enum_value;
    }


    const Transmitter_Technology_bit_7_4_string& getSFF8636_Transmitter_Technology_bit_7_4Info(Transmitter_Technology_bit_7_4 enum_value) {
        return *std::find_if(
            Transmitter_Technology_bit_7_4_strings.begin(),
            Transmitter_Technology_bit_7_4_strings.end(),
            [enum_value](const Transmitter_Technology_bit_7_4_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    Transmitter_Technology_bit_7_4 getSFF8636_Transmitter_Technology_bit_7_4_From_Char(unsigned char value) {
        return std::find_if(
            Transmitter_Technology_bit_7_4_strings.begin(),
            Transmitter_Technology_bit_7_4_strings.end(),
            [value](const Transmitter_Technology_bit_7_4_string& entry) { return entry.bitValue == ((value >> 4) & 0b00001111 ); }
        )->enum_value;
    }
}