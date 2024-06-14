#include <algorithm>
#include <string>
#include "fmt/core.h"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8024_Encoding_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Extended_Compliance_Codes.hpp"

namespace TransceiverTool::Standards::SFF8024 {
    std::string byteToTransceiverReferenceString(unsigned char byte) {
        auto it = std::find_if(
            TransceiverReferenceAssignedValues.begin(),
            TransceiverReferenceAssignedValues.end(),
            [byte](const TransceiverReferenceAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != TransceiverReferenceAssignedValues.end()) {
            name = it->name;
        } else if(byte <= 0x7F) {
            name = "Reserved";
        } else {
            name = "Vendor Specific";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }

    std::string byteToTransceiverConnectorTypeString(unsigned char byte) {
        auto it = std::find_if(
            TransceiverConnectorTypeAssignedValues.begin(),
            TransceiverConnectorTypeAssignedValues.end(),
            [byte](const TransceiverConnectorTypeAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != TransceiverConnectorTypeAssignedValues.end()) {
            name = it->name;
        } else {
            name = "Reserved";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }

    std::string byteToTransceiverEncodingString(unsigned char byte) {
        auto it = std::find_if(
            SFF8636TransceiverEncodingAssignedValues.begin(),
            SFF8636TransceiverEncodingAssignedValues.end(),
            [byte](const SFF8636TransceiverEncodingAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != SFF8636TransceiverEncodingAssignedValues.end()) {
            name = it->name;
        } else {
            name = "Reserved";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }

    std::string byteToExtendedComplianceCodeString(unsigned char byte) {
        auto it = std::find_if(
            ExtendedComplianceCodesAssignedValues.begin(),
            ExtendedComplianceCodesAssignedValues.end(),
            [byte](const ExtendedComplianceCodesAssignedValue& it_val) { return it_val.byte_value == byte; }
        );

        std::string name;
        if(it != ExtendedComplianceCodesAssignedValues.end()) {
            name = it->name;
        } else {
            name = "Reserved";
        }

        return fmt::format("{} ({:#04x})", name, byte);
    }
}