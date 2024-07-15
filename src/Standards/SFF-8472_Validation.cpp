#include "TransceiverTool/Standards/SFF-8472_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
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
                        20 + index, index, programming.byte_20_35_vendor_name[index]
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

    void validateVendorPartNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Section 7.3 Vendor PN [Address A0h, Bytes 40-55]
        bool vendorPNAllZeros = std::all_of(programming.byte_40_55_vendor_pn.begin(), programming.byte_40_55_vendor_pn.end(), [](unsigned char val) { return val == 0; });
        if(!vendorPNAllZeros) {
            for(int index = 0; index < programming.byte_40_55_vendor_pn.size(); ++index) {
                if(!std::isprint(programming.byte_40_55_vendor_pn[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor PN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            40 + index, index, programming.byte_40_55_vendor_pn[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor PN field is not left aligned, padded with spaces (0x20)?
    }

    void validateVendorRevisionNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Section 7.4 Vendor Rev [Address A0h, Bytes 56-59
        bool vendorRevAllZeros = std::all_of(programming.byte_56_59_vendor_rev.begin(), programming.byte_56_59_vendor_rev.end(), [](unsigned char val) { return val == 0; });
        if(!vendorRevAllZeros) {
            for(int index = 0; index < programming.byte_56_59_vendor_rev.size(); ++index) {
                if(!std::isprint(programming.byte_56_59_vendor_rev[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor Rev\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            56 + index, index, programming.byte_56_59_vendor_rev[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor Rev field is not left aligned, padded with spaces (0x20)?
    }

    void validateWavelengthOrCableSpecificationCompliance(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        if(programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
            validationResult.errors.push_back(
                "Both Passive Cable Compliance Bit (Byte 8, Bit 2) and Active Cable Compliance Bit (Byte 8, Bit 3) are set!"
            );
            return;
        }

        if(!programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && !programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
            //Bytes are wavelength -> Nothing to validate
        }
        if(programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && !programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
            //Passive cable
            if(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & 0b11111100) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 60 (\"Passive Cable Specification Compliance\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}",
                        bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7)), bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6)), 
                        bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5)), bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4)),
                        bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 3)), bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 2)) 
                    )
                );
            }
            if(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 61 (\"Passive Cable Specification Compliance\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6)), 
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4)),
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2)),
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0)) 
                    )
                );
            }
        }
        if(!programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
            //Passive cable
            if(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & 0b11110000) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 60 (\"Active Cable Specification Compliance\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}",
                        bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7)), bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6)), 
                        bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5)), bool(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4))
                    )
                );
            }
            if(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 61 (\"Active Cable Specification Compliance\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6)), 
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4)),
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2)),
                        bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1)), bool(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0)) 
                    )
                );
            }
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

    void validateCC_BASEChecksum(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        std::vector<unsigned char> buffer; buffer.resize(128, 0x00);
        assembleToBinary(buffer.data(), programming, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);

        auto correctChecksum = calculateCC_BASEChecksum(buffer.data());

        if(programming.byte_63_CC_BASE != correctChecksum) {
            validationResult.errors.push_back(
                fmt::format("Byte 63 (\"CC_EXT\") value is {:#04x}, but should be {:#04x}", programming.byte_63_CC_BASE, correctChecksum)
            );
        }
    }

    void validateOptionValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        if(programming.byte_64_option_values.reserved_bit_7) {
            validationResult.errors.push_back(
                "Byte 64 (\"Option Values\") value has reserved bit 7 set"
            );
        }
        if(programming.byte_65_option_values.loss_of_signal_inverted_bit_2 && programming.byte_65_option_values.loss_of_signal_implemented_bit_1) {
            validationResult.errors.push_back(
                "Byte 65 (\"Option Values\") value has both Loss of Signal implemented, signal inverted (Bit 2) and Loss of Signal implemented, behavior as defined in SFF-8419 (Bit 1) set"
            );
        }
        if(programming.byte_65_option_values.reserved_bit_0) {
            validationResult.errors.push_back(
                "Byte 65 (\"Option Values\") value has reserved bit 0 set"
            );
        }
    }
    
    void validateVendorSerialNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Section 8.6 Vendor SN [Address A0h, Bytes 68-83]
        bool vendorSNAllZeros = std::all_of(programming.byte_68_83_vendor_sn.begin(), programming.byte_68_83_vendor_sn.end(), [](unsigned char val) { return val == 0; });
        if(!vendorSNAllZeros) {
            for(int index = 0; index < programming.byte_68_83_vendor_sn.size(); ++index) {
                if(!std::isprint(programming.byte_68_83_vendor_sn[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor SN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            68 + index, index, programming.byte_68_83_vendor_sn[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor SN field is not left aligned, padded with spaces (0x20)?
    }

    void validateDateCode(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        //SFF-8472 Rev 12.4 Section 8.7 Date Code [Address A0h, Bytes 84-91]
        if(
            !std::isdigit(programming.byte_84_91_date_code.year_low_order_digits[0]) || 
            !std::isdigit(programming.byte_84_91_date_code.year_low_order_digits[1])
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 84 / 85 (\"Date Code\", low order digits of the year) is not a number"
                )
            );
        }
        //No need to parse: We checked that both characters are numbers [0-9] and all values are valid (year 2000 - 2099)

        if(
            programming.byte_84_91_date_code.month_digits[0] <= 0x2F || programming.byte_84_91_date_code.month_digits[0] >= 0x3A ||
            programming.byte_84_91_date_code.month_digits[1] <= 0x2F || programming.byte_84_91_date_code.month_digits[1] >= 0x3A
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 86 / 87 (\"Date Code\", digits of the month) is not a number"
                )
            );
        } else {
            try {
                int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.month_digits.data()), 2));
                if(digits_of_month < 1 || digits_of_month > 12) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte 86 / 87 (\"Date Code\", digits of the month) must be within [0;12] but is {}",
                            digits_of_month
                        )
                    );
                }
            } catch(const std::exception& e) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 86 / 87 (\"Date Code\", digits of the month) is not a valid number (could not parse?)"
                    )
                );
            }
        }

        if(
            programming.byte_84_91_date_code.day_digits[0] <= 0x2F || programming.byte_84_91_date_code.day_digits[0] >= 0x3A ||
            programming.byte_84_91_date_code.day_digits[1] <= 0x2F || programming.byte_84_91_date_code.day_digits[1] >= 0x3A
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 88 / 89 (\"Date Code\", day of the month) is not a number"
                )
            );
        } else {
            try {
                int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.day_digits.data()), 2));
                if(digits_of_month < 1 || digits_of_month > 31) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte 88 / 89 (\"Date Code\", day of the month) must be within [0;31] but is {}",
                            digits_of_month
                        )
                    );
                }
            } catch(const std::exception& e) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 88 / 89 (\"Date Code\", day of the month) is not a valid number (could not parse?)"
                    )
                );
            }
        }

        for(int index = 0; index < programming.byte_84_91_date_code.lot_code.size(); ++index) {
            if(!std::isprint(programming.byte_84_91_date_code.lot_code[index])) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor Lot Code\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        90 + index, index, programming.byte_84_91_date_code.lot_code[index]
                    )
                );
            }
        }
    }

    void validateCC_EXTChecksum(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult) {
        std::vector<unsigned char> buffer; buffer.resize(128, 0x00);
        assembleToBinary(buffer.data(), programming, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);

        auto correctChecksum = calculateCC_EXTChecksum(buffer.data());

        if(programming.byte_95_CC_EXT != correctChecksum) {
            validationResult.errors.push_back(
                fmt::format("Byte 95 (\"CC_EXT\") value is {:#04x}, but should be {:#04x}", programming.byte_95_CC_EXT, correctChecksum)
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

        //SFF-8472 Rev 12.4 Section 7.3 Vendor PN [Address A0h, Bytes 40-55]
        validateVendorPartNumber(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 7.4 Vendor Rev [Address A0h, Bytes 56-59]
        validateVendorRevisionNumber(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.1 Optical and Cable Variants Specification Compliance [Address A0h, Bytes 60-61]
        validateWavelengthOrCableSpecificationCompliance(programming, validationResult);

        //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
        validateFibreChannelSpeed2ComplianceCodes(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.2 CC_BASE [Address A0h, Byte 63]
        validateCC_BASEChecksum(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.3 Option Values [Address A0h, Bytes 64-65]
        validateOptionValues(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.6 Vendor SN [Address A0h, Bytes 68-83]
        validateVendorSerialNumber(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.7 Date Code [Address A0h, Bytes 84-91]
        validateDateCode(programming, validationResult);

        //SFF-8472 Rev 12.4 Section 8.12 CC_EXT [Address A0h, Byte 95]
        validateCC_EXTChecksum(programming, validationResult);

        return validationResult;
    }
}