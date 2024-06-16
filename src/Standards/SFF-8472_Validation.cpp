#include "TransceiverTool/Standards/SFF-8472_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include <cctype>
#include <fmt/core.h>
#include <algorithm>

namespace TransceiverTool::Standards::SFF8472::Validation {

    void validateIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
        if(programming.byte_0_Identifier >= 0x04 && programming.byte_0_Identifier <= 0x7F) {
            validationResult.errors.push_back(
                fmt::format("Byte 0 (\"Identifier\") value corresponds to \"Not used by this specification\" / \"Reserved\" range (SFF-8472 Rev 12.4 Table 5-1 \"Physical Device Identifier Values\"), value is {:#04x}", programming.byte_0_Identifier)
            );
        }
        if(programming.byte_0_Identifier >= 0x80) {
            validationResult.warnings.push_back(
                fmt::format("Byte 0 (\"Identifier\") value corresponds to \"Vendor Specific\" range (SFF-8472 Rev 12.4 Table 5-1 \"Physical Device Identifier Values\"), value is {:#04x}", programming.byte_0_Identifier)
            );
        }
    }

    void validateExtendedIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Table 5-2 Physical Device Extended Identifier Values
        if(programming.byte_1_extended_identifier >= 0x08) {
            validationResult.errors.push_back(
                fmt::format("Byte 1 (\"Extended Identifier\") value corresponds to \"Reserved\" range (SFF-8472 Rev 12.4 Table 5-2 \"Physical Device Extended Identifier Values\"), value is {:#04x}", programming.byte_1_extended_identifier)
            );
        }
    }

    void validateConnectorTypes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-3 Connector Types
        if(
            (programming.byte_2_Connector_type >= 0x0E && programming.byte_2_Connector_type <= 0x1F) ||
            (programming.byte_2_Connector_type >= 0x29 && programming.byte_2_Connector_type <= 0x7F)
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 2 (\"Connector Type\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}",
                    programming.byte_2_Connector_type
                )
            );
        }
        if(programming.byte_2_Connector_type >= 0x80) {
            validationResult.warnings.push_back(
                fmt::format(
                    "Byte 2 (\"Connector Type\") value corresponds to \"Vendor specific\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}",
                    programming.byte_2_Connector_type
                )
            );
        }
    }

    void validateSONETComplianceCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        if(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7 || 
            programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 5 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 3 {:d}",
                    programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7, programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3
                )
            );
        }
    }

    //TODO: Introduce options to not warn on values in "Vendor Specific" ranges (in case this tool is used by an actual vendor?)
    common::ValidationResult validateSFF8472_LowerA0h(const TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h& programming) {
        common::ValidationResult validationResult;

        //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
        validateIdentifierValues(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-2 Physical Device Extended Identifier Values
        validateExtendedIdentifierValues(programming, validationResult);

        //SFF-8024 Rev 4.11 Table 4-3 Connector Types
        validateConnectorTypes(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        validateSONETComplianceCodes(programming, validationResult);

        return validationResult;
    }
}