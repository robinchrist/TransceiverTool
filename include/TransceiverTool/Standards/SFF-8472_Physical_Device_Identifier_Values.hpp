#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    enum class PhysicalDeviceIdentifier {
        Unknown_or_unspecified,
        GBIC,
        Module_soldered_to_motherboard_ex_SFF,
        SFP_or_SFP_plus
    };

    struct PhysicalDeviceIdentifierAssignedValue {
        PhysicalDeviceIdentifier enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    inline const std::array<PhysicalDeviceIdentifierAssignedValue, 4> PhysicalDeviceIdentifierAssignedValues {{
        {PhysicalDeviceIdentifier::Unknown_or_unspecified, "Unknown or unspecified", 0x00},
        {PhysicalDeviceIdentifier::GBIC, "GBIC", 0x01},
        {PhysicalDeviceIdentifier::Module_soldered_to_motherboard_ex_SFF, "Module soldered to motherboard (ex: SFF)", 0x02},
        {PhysicalDeviceIdentifier::SFP_or_SFP_plus, "SFP or SFP+", 0x03}
        //0x04 - 0x7F: Not used by this specification (Reserved)
        //0x80 - 0xFF: Vendor Specific 
    }};

    std::string byteToPhysicalDeviceIdentifierString(unsigned char byte);

}