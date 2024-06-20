#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Rate_Identifiers.hpp"
#include "fmt/core.h"
#include <algorithm>

namespace TransceiverTool::Standards::SFF8472 {

    std::string byteToPhysicalDeviceIdentifierString(unsigned char byte) {
        auto it = std::find_if(
            PhysicalDeviceIdentifierAssignedValues.begin(),
            PhysicalDeviceIdentifierAssignedValues.end(),
            [byte](const PhysicalDeviceIdentifierAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != PhysicalDeviceIdentifierAssignedValues.end()) {
            name = it->name;
        } else if(byte <= 0x7F) {
            name = "Reserved / Not used by this specification";
        } else {
            name = "Vendor Specific";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }

    std::string byteToPhysicalDeviceExtendedIdentifierString(unsigned char byte) {
        auto it = std::find_if(
            PhysicalDeviceExtendedIdentifierAssignedValues.begin(),
            PhysicalDeviceExtendedIdentifierAssignedValues.end(),
            [byte](const PhysicalDeviceExtendedIdentifierAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != PhysicalDeviceExtendedIdentifierAssignedValues.end()) {
            name = it->name;
        } else {
            name = "Reserved";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }

    const SONETReachSpecifierAssignedValue& getSFF8472_SONETReachSpecifierInfo(SONETReachSpecifier enum_value) {
        return *std::find_if(
            SONETReachSpecifierAssignedValues.begin(),
            SONETReachSpecifierAssignedValues.end(),
            [enum_value](const SONETReachSpecifierAssignedValue& entry) { return entry.enum_value == enum_value; }
        );
    }
    SONETReachSpecifier getSFF8472_SONETReachSpecifier_From_Char(unsigned char value) {
        return std::find_if(
            SONETReachSpecifierAssignedValues.begin(),
            SONETReachSpecifierAssignedValues.end(),
            [value](const SONETReachSpecifierAssignedValue& entry) { return entry.byte_value == ((value >> 3) & 0b00000011 ); }
        )->enum_value;
    }

    const RateIdentifierAssignedValue& getSFF8472_RateIdentifierInfo(RateIdentifier enum_value) {
        return *std::find_if(
            RateIdentifierAssignedValues.begin(),
            RateIdentifierAssignedValues.end(),
            [enum_value](const RateIdentifierAssignedValue& entry) { return entry.enum_value == enum_value; }
        );
    }
    RateIdentifier getSFF8472_RateIdentifier_From_Char(unsigned char value) {
         return std::find_if(
            RateIdentifierAssignedValues.begin(),
            RateIdentifierAssignedValues.end(),
            [value](const RateIdentifierAssignedValue& entry) { return entry.byte_value == value; }
        )->enum_value;
    }
    std::string byteToRateIdentifierString(unsigned char byte) {
        auto it = std::find_if(
            RateIdentifierAssignedValues.begin(),
            RateIdentifierAssignedValues.end(),
            [byte](const RateIdentifierAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != RateIdentifierAssignedValues.end()) {
            name = it->name;
        } else if (byte == 0x03 || byte == 0x05 || byte == 0x07 || byte == 0x09 || byte == 0x0B || byte == 0x0D || byte == 0x0F || byte == 0x11) {
            name = "Unspecified or INF-8074 (value = 0) or 4/2/1G selection per SFF-8079 (value = 1)";
        } else { // (byte >= 0x12 && byte <= 0x1F) || (byte >= 0x21 && byte <= 0xFF)
            name = "Reserved";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }
}