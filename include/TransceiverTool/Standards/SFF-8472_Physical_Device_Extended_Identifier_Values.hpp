#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    enum class PhysicalDeviceExtendedIdentifier {
        definition_is_not_specified,
        compliant_with_MOD_DEF_1,
        compliant_with_MOD_DEF_2,
        compliant_with_MOD_DEF_3,
        defined_by_2_wire_interface_id_only,
        compliant_with_MOD_DEF_5,
        compliant_with_MOD_DEF_6,
        compliant_with_MOD_DEF_7
    };

    struct PhysicalDeviceExtendedIdentifierAssignedValue {
        PhysicalDeviceExtendedIdentifier enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    inline const std::array<PhysicalDeviceExtendedIdentifierAssignedValue, 8> PhysicalDeviceExtendedIdentifierAssignedValues {{
        {PhysicalDeviceExtendedIdentifier::definition_is_not_specified, "GBIC definition is not specified or the GBIC definition is not compliant with a defined MOD_DEF. See product specification for details.", 0x00},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_1, "GBIC is compliant with MOD_DEF 1", 0x01},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_2, "GBIC is compliant with MOD_DEF 2", 0x02},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_3, "GBIC is compliant with MOD_DEF 3", 0x03},
        {PhysicalDeviceExtendedIdentifier::defined_by_2_wire_interface_id_only, "GBIC/SFP function is defined by 2-wire interface ID only", 0x04},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_5, "GBIC is compliant with MOD_DEF 5", 0x05},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_6, "GBIC is compliant with MOD_DEF 6", 0x06},
        {PhysicalDeviceExtendedIdentifier::compliant_with_MOD_DEF_7, "GBIC is compliant with MOD_DEF 7", 0x07},
        //0x08 - 0xFF: Reserved
    }};

    std::string byteToPhysicalDeviceExtendedIdentifierString(unsigned char byte);

}