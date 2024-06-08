#include <cstddef>
#include <exception>
#include <variant>
#include <string>
#include <array>
#include <vector>
#include <fmt/core.h>
#include <algorithm>

#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {
    const Extended_Identifier_Bit_7_6_string& getSFF8636_Extended_Identifier_Bit_7_6Info(Extended_Identifier_Bit_7_6 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_7_6_strings.begin(),
            Extended_Identifier_Bit_7_6_strings.end(),
            [enum_value](const Extended_Identifier_Bit_7_6_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    const Extended_Identifier_Bit_1_0_string& getSFF8636_Extended_Identifier_Bit_1_0Info(Extended_Identifier_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_1_0_strings.begin(),
            Extended_Identifier_Bit_1_0_strings.end(),
            [enum_value](const Extended_Identifier_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    const Extended_Rate_Select_Compliance_Bit_1_0_string& getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(Extended_Rate_Select_Compliance_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
            Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
            [enum_value](const Extended_Rate_Select_Compliance_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }
}




struct ValidationResult {
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};



int main() {
    
    return 0;
}