#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
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

}