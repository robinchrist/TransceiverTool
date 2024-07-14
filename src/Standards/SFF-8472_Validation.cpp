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

    void validateSFPPlusCableTechnologyCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        if(programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_1 || programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_0) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 8 (\"SFP+ Cable Technology\") value has at least one reserved bit set: Bit 1 {:d}, Bit 0 {:d}",
                    programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_1, programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_0
                )
            );
        }
    }

    void validateFibreChannelTransmissionMediaCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        if(programming.byte_9_fibre_channel_transmission_media_codes.reserved_bit_1) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 9 (\"Fibre Channel Transmission Media\") has reserved bit 1 set"
                )
            );
        }
    }   

    void validateEncodingValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
        if(programming.byte_11_Encoding >= 0x09) {
            validationResult.errors.push_back(
                fmt::format("Byte 11 (\"Encoding\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-2 \"Encoding Values\"), value is {:#04x}", programming.byte_11_Encoding)
            );
        }
    }

    void validateSignalingRate(const SFF8472_LowerA0h &programming, common::ValidationResult &validationResult) {
        //SFF-8472 Rev 12.4 Section 5.6 Signaling rate, nominal [Address A0h, Byte 12] & Section 8.4 Signaling Rate, max [Address A0h, Byte 66]
        if(programming.byte_12_nominal_signaling_rate_in_100_mbaud == 0) {
            validationResult.warnings.push_back(
                fmt::format("Byte 12 (\"Nominal Signaling Rate\") value is 0")
            );
        }
        if(programming.byte_12_nominal_signaling_rate_in_100_mbaud == 0xFF) {
            if(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud == 0) {
                validationResult.warnings.push_back(
                    fmt::format("Byte 12 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but Byte 66 (\"Extended Baud Rate: Nominal\") is zero")
                );
            } else {
                unsigned long nominalBaudRateFromByte66 = (unsigned long)(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250UL;

                if(nominalBaudRateFromByte66 < 25400) {
                    validationResult.warnings.push_back(
                        fmt::format("Byte 12 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but value from Byte 66 (\"Extended Baud Rate: Nominal\") is too small ({} MBd)", nominalBaudRateFromByte66)
                    );
                }
            }
        }
        if(programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent > 100) {
            validationResult.warnings.push_back(
                fmt::format("Byte 66 (\"Signaling Rate, min\") exceeds 100% which would result in negative baud rates")
            );
        }
    }

    void validateRateIdentifier(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Table 5-6 Rate Identifier
        auto byte = programming.byte_13_rate_identifier;
        if(byte == 0x03 || byte == 0x05 || byte == 0x07 || byte == 0x09 || byte == 0x0B || byte == 0x0D || byte == 0x0F || byte == 0x11) {
            validationResult.warnings.push_back(
                fmt::format("Byte 13 (\"Rate Identifier\") value corresponds to \"Unspecified\" legacy range (SFF-8472 Rev 12.4 Table 5-6 \"Rate Identifier\"), value is {:#04x}", programming.byte_11_Encoding)
            );
        }
        if((byte >= 0x12 && byte <= 0x1F) || byte >= 0x21) {
            validationResult.errors.push_back(
                fmt::format("Byte 13 (\"Rate Identifier\") value corresponds to \"Reserved\" range (SFF-8472 Rev 12.4 Table 5-6 \"Rate Identifier\"), value is {:#04x}", programming.byte_11_Encoding)
            );
        }
    }

    void validateVendorName(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        for(int index = 0; index < programming.byte_20_35_vendor_name.size(); ++index) {
            if(!std::isprint(programming.byte_20_35_vendor_name[index])) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor Name\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        148 + index, index, programming.byte_20_35_vendor_name[index]
                    )
                );
            }
        }
        //TODO: Warn if Vendor Name field is not left aligned, padded with spaces (0x20)?
    }

    void validateExtendedSpecificationComplianceCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
        if(programming.byte_36_extended_specification_compliance_codes == 0x0A || programming.byte_36_extended_specification_compliance_codes == 0x0F ||
            (programming.byte_36_extended_specification_compliance_codes >= 0x3B && programming.byte_36_extended_specification_compliance_codes <= 0x3E) ||
            (programming.byte_36_extended_specification_compliance_codes >= 0x4D && programming.byte_36_extended_specification_compliance_codes <= 0x7E) ||
            programming.byte_36_extended_specification_compliance_codes >= 0x82
        ) {
            validationResult.errors.push_back(
                fmt::format("Byte 36 (\"Extended Specification Compliance Codes\") value corresponds to \"Reserved\" range / value (SFF-8024 Rev 4.11 Table 4-4 \"Extended Specification Compliance Codes\"), value is {:#04x}", programming.byte_36_extended_specification_compliance_codes)
            );
        }
    }

    void validateFibreChannelSpeed2ComplianceCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        if(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_7 || programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_6 ||
            programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_5 || programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_4 ||
            programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_3 || programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_2 ||
            programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_1
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 62 (\"Fibre Channel Speed 2\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}",
                    programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_7, programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_6, 
                    programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_5, programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_4, 
                    programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_3, programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_2, 
                    programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_1
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

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        validateSFPPlusCableTechnologyCodes(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        validateFibreChannelTransmissionMediaCodes(programming, validationResult);

        //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
        validateEncodingValues(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 5.6 Signaling rate, nominal [Address A0h, Byte 12] & Section 8.4 Signaling Rate, max [Address A0h, Byte 66]
        validateSignalingRate(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-6 Rate Identifier
        validateRateIdentifier(programming, validationResult);

        //Nope I'm definitely not implementing validation for the Link Length stuff in bytes [14-19]
        //This section is so random and messed up that I'm not spending more time with it for the sake of my mental health

        //SFF-8472 Rev 12.4 Section 7.1 Vendor name [Address A0h, Bytes 20-35]
        validateVendorName(programming, validationResult);

        //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
        validateExtendedIdentifierValues(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        validateFibreChannelSpeed2ComplianceCodes(programming, validationResult);

        return validationResult;
    }
}