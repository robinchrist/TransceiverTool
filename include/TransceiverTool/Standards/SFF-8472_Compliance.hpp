#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    enum class SFF_8472_Compliance {
        SFF_8472_Rev_9_3,
        SFF_8472_Rev_9_5,
        SFF_8472_Rev_10_2,
        SFF_8472_Rev_10_4,
        SFF_8472_Rev_11_0,
        SFF_8472_Rev_11_3,
        SFF_8472_Rev_11_4,
        SFF_8472_Rev_12_3,
        SFF_8472_Rev_12_4,
    };

    struct SFF_8472_ComplianceAssignedValue {
        SFF_8472_Compliance enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8472 Rev 12.4 Section 8.11 SFF-8472 Compliance [Address A0h, Byte 94]
    inline const std::array<SFF_8472_ComplianceAssignedValue, 9> SFF_8472_ComplianceAssignedValues {{
        //0x00: ??
        {SFF_8472_Compliance::SFF_8472_Rev_9_3, "Includes functionality described in Rev 9.3 of SFF-8472", 0x01},
        {SFF_8472_Compliance::SFF_8472_Rev_9_5, "Includes functionality described in Rev 9.5 of SFF-8472", 0x02},
        {SFF_8472_Compliance::SFF_8472_Rev_10_2, "Includes functionality described in Rev 10.2 of SFF-8472", 0x03},
        {SFF_8472_Compliance::SFF_8472_Rev_10_4, "Includes functionality described in Rev 10.4 of SFF-8472", 0x04},
        {SFF_8472_Compliance::SFF_8472_Rev_11_0, "Includes functionality described in Rev 11.0 of SFF-8472", 0x05},
        {SFF_8472_Compliance::SFF_8472_Rev_11_3, "Includes functionality described in Rev 11.3 of SFF-8472", 0x06},
        {SFF_8472_Compliance::SFF_8472_Rev_11_4, "Includes functionality described in Rev 11.4 of SFF-8472", 0x07},
        {SFF_8472_Compliance::SFF_8472_Rev_12_3, "Includes functionality described in Rev 12.3 of SFF-8472", 0x08},
        {SFF_8472_Compliance::SFF_8472_Rev_12_4, "Includes functionality described in Rev 12.4 of SFF-8472", 0x09},
        //0x0A - 0xFF: Reserved
    }};
    const SFF_8472_ComplianceAssignedValue& getSFF_8472_ComplianceInfo(SFF_8472_Compliance enum_value);
    std::string byteToSFF_8472_ComplianceAssignedValueString(unsigned char byte);


}