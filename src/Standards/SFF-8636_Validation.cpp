#include "TransceiverTool/Standards/SFF-8636_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Rate_Select_Compliance.hpp"
#include <cctype>
#include <fmt/core.h>
#include <algorithm>

namespace TransceiverTool::Standards::SFF8636::Validation {

    void validateIdentifierValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
        if(programming.byte_128_Identifier >= 0x26 && programming.byte_128_Identifier <= 0x7F) {
            validationResult.errors.push_back(
                fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
            );
        }
        if(programming.byte_128_Identifier >= 0x80) {
            validationResult.warnings.push_back(
                fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Vendor Specific\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
            );
        }
    }

    void validateExtendedIdentifierValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129)
        //If bit 1-0 is not 00 (i.e. Power Class 5/6/7), bit 7-6 must indicate 11 (Power Class 4-7)
        if(programming.byte_129_extended_identifier.power_class_bit_1_0 != TransceiverTool::Standards::SFF8636::Extended_Identifier_Bit_1_0::POWER_CLASSES_1_TO_4_0b00 &&
            programming.byte_129_extended_identifier.power_class_bit_7_6 != TransceiverTool::Standards::SFF8636::Extended_Identifier_Bit_7_6::POWER_CLASS_4_TO_7_0b11
        ) {
            validationResult.errors.push_back(
                //TODO: add bits value present in programming to error message?
                fmt::format("Byte 129 (\"Extended Identifier\") inconsistent values: value bits 1-0 indicate power class \"{}\", but bits 7-6 indicates \"{}\"", 
                    getSFF8636_Extended_Identifier_Bit_1_0Info(programming.byte_129_extended_identifier.power_class_bit_1_0).description,
                    getSFF8636_Extended_Identifier_Bit_7_6Info(programming.byte_129_extended_identifier.power_class_bit_7_6).description
                )
            );
        }
    }

    void validateConnectorTypes(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-3 Connector Types
        if(
            (programming.byte_130_Connector_Type >= 0x0E && programming.byte_130_Connector_Type <= 0x1F) ||
            (programming.byte_130_Connector_Type >= 0x29 && programming.byte_130_Connector_Type <= 0x7F)
        ) {
            validationResult.errors.push_back(
                fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
            );
        }
        if(programming.byte_130_Connector_Type >= 0x80) {
            validationResult.warnings.push_back(
                fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Vendor specific\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
            );
        }
    }

    void validateSpecificationComplianceCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
        //Byte 132: SONET Compliance Codes
        if(programming.byte_132_sonet_compliance_codes.reserved_bit_7 || programming.byte_132_sonet_compliance_codes.reserved_bit_6 ||
            programming.byte_132_sonet_compliance_codes.reserved_bit_5 || programming.byte_132_sonet_compliance_codes.reserved_bit_4 ||
            programming.byte_132_sonet_compliance_codes.reserved_bit_3
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 132 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}",
                    programming.byte_132_sonet_compliance_codes.reserved_bit_7, programming.byte_132_sonet_compliance_codes.reserved_bit_6,
                    programming.byte_132_sonet_compliance_codes.reserved_bit_5, programming.byte_132_sonet_compliance_codes.reserved_bit_4,
                    programming.byte_132_sonet_compliance_codes.reserved_bit_3
                )
            );
        }

        //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
        //Byte 133: SAS/SATA Compliance Codes
        if(programming.byte_133_sas_sata_compliance_codes.reserved_bit_3 || programming.byte_133_sas_sata_compliance_codes.reserved_bit_2 ||
            programming.byte_133_sas_sata_compliance_codes.reserved_bit_1 || programming.byte_133_sas_sata_compliance_codes.reserved_bit_0
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 133 (\"SAS/SATA Compliance Codes\") value has at least one reserved bit set: Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                    programming.byte_133_sas_sata_compliance_codes.reserved_bit_3, programming.byte_133_sas_sata_compliance_codes.reserved_bit_2, 
                    programming.byte_133_sas_sata_compliance_codes.reserved_bit_1, programming.byte_133_sas_sata_compliance_codes.reserved_bit_0
                )
            );
        }

        //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
        //Byte 134: Gigabit Ethernet Compliance Codes
        if(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7 || programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6 ||
            programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5 || programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 134 (\"Gigabit Ethernet Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}",
                    programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7, programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6,
                    programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5, programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4
                )
            );
        }

        //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
        //Byte 136: Fibre Channel Transmitter Technology
        if(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3 || programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2 ||
            programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1 || programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 136 (\"Fibre Channel Transmitter Technology\") value has at least one reserved bit set: Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                    programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2, 
                    programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0
                )
            );
        }
    }

    void validateEncodingValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
        if(programming.byte_139_Encoding >= 0x09) {
            validationResult.errors.push_back(
                fmt::format("Byte 139 (\"Encoding\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-2 \"Encoding Values\"), value is {:#04x}", programming.byte_139_Encoding)
            );
        }
    }

    void validateNominalSignalingRate(const SFF8636_Upper00h &programming, ValidationResult &validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.6 Nominal Signaling Rate (00h 140) & Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
        if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0) {
            validationResult.warnings.push_back(
                fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0")
            );
        }
        if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0xFF) {
            if(programming.byte_222_extended_baud_rate_in_250_mbaud == 0) {
                validationResult.warnings.push_back(
                    fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but Byte 222 (\"Extended Baud Rate: Nominal\") is zero")
                );
            } else {
                unsigned long nominalBaudRateFromByte222 = (unsigned long)(programming.byte_222_extended_baud_rate_in_250_mbaud) * 250UL;

                if(nominalBaudRateFromByte222 < 25400) {
                    validationResult.warnings.push_back(
                        fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but value from Byte 222 (\"Extended Baud Rate: Nominal\") is too small ({} MBd)", nominalBaudRateFromByte222)
                    );
                }
            }
        }
    }

    void validateExtendedRateSelectCompliance(const SFF8636_Upper00h &programming, ValidationResult &validationResult) {
        //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
        if(programming.byte_141_extended_rate_select_compliance.reserved_bit_7 || programming.byte_141_extended_rate_select_compliance.reserved_bit_6 ||
            programming.byte_141_extended_rate_select_compliance.reserved_bit_5 || programming.byte_141_extended_rate_select_compliance.reserved_bit_4 ||
            programming.byte_141_extended_rate_select_compliance.reserved_bit_3 || programming.byte_141_extended_rate_select_compliance.reserved_bit_2 ||
            programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == TransceiverTool::Standards::SFF8636::Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b11
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 132 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1-0: \"{}\"",
                    programming.byte_141_extended_rate_select_compliance.reserved_bit_7, programming.byte_141_extended_rate_select_compliance.reserved_bit_6,
                    programming.byte_141_extended_rate_select_compliance.reserved_bit_5, programming.byte_141_extended_rate_select_compliance.reserved_bit_4,
                    programming.byte_141_extended_rate_select_compliance.reserved_bit_3, programming.byte_141_extended_rate_select_compliance.reserved_bit_2,
                    TransceiverTool::Standards::SFF8636::getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0).description
                )
            );
        }
    }

    void validateVendorName(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        for(int index = 0; index < programming.byte_148_163_vendor_name.size(); ++index) {
            if(!std::isprint(programming.byte_148_163_vendor_name[index])) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor Name\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        148 + index, index, programming.byte_148_163_vendor_name[index]
                    )
                );
            }
        }
        //TODO: Warn if Vendor Name field is not left aligned, padded with spaces (0x20)?
    }

    void validateExtendedModuleCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Table 6-21 Extended Module Code Values (Page 00h Byte 164)
        if(programming.byte_164_extended_module_codes.reserved_bit_7 || programming.byte_164_extended_module_codes.reserved_bit_6) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 164 (\"Extended Module Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}",
                    programming.byte_164_extended_module_codes.reserved_bit_7, programming.byte_164_extended_module_codes.reserved_bit_6
                )
            );
        }
    }

    void validateVendorPartNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.17 Vendor Part Number (00h 168-183)
        bool vendorPNAllZeros = std::all_of(programming.byte_168_183_vendor_pn.begin(), programming.byte_168_183_vendor_pn.end(), [](unsigned char val) { return val == 0; });
        if(!vendorPNAllZeros) {
            for(int index = 0; index < programming.byte_168_183_vendor_pn.size(); ++index) {
                if(!std::isprint(programming.byte_168_183_vendor_pn[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor PN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            168 + index, index, programming.byte_168_183_vendor_pn[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor PN field is not left aligned, padded with spaces (0x20)?
    }

    void validateVendorRevisionNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.18 Vendor Revision Number (00h 184-185)
        bool vendorRevAllZeros = std::all_of(programming.byte_184_185_vendor_rev.begin(), programming.byte_184_185_vendor_rev.end(), [](unsigned char val) { return val == 0; });
        if(!vendorRevAllZeros) {
            for(int index = 0; index < programming.byte_184_185_vendor_rev.size(); ++index) {
                if(!std::isprint(programming.byte_184_185_vendor_rev[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor Rev\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            184 + index, index, programming.byte_184_185_vendor_rev[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor Rev field is not left aligned, padded with spaces (0x20)?
    }

    void validateMaximumCaseTemperature(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.21 Maximum Case Temperature (00h 190)
        if(programming.byte_190_max_case_temperature == 70) {
            validationResult.warnings.push_back(
                fmt::format(
                    "Byte 190 (\"Maximum Case Temperature\") value is {:#04x} -> 70 deg C, but standards mandates setting to 00h for 70 deg C and values other than 00h must only be used for maximum case temperatures that are not 70 deg C",
                    programming.byte_190_max_case_temperature
                )
            );
        }
    }

    void validateExtendedSpecificationComplianceCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
        if(programming.byte_192_extended_specification_compliance_codes == 0x0A || programming.byte_192_extended_specification_compliance_codes == 0x0F ||
            (programming.byte_192_extended_specification_compliance_codes >= 0x3B && programming.byte_192_extended_specification_compliance_codes <= 0x3E) ||
            (programming.byte_192_extended_specification_compliance_codes >= 0x4D && programming.byte_192_extended_specification_compliance_codes <= 0x7E) ||
            programming.byte_192_extended_specification_compliance_codes >= 0x82
        ) {
            validationResult.errors.push_back(
                fmt::format("Byte 192 (\"Extended Specification Compliance Codes\") value corresponds to \"Reserved\" range / value (SFF-8024 Rev 4.11 Table 4-4 \"Extended Specification Compliance Codes\"), value is {:#04x}", programming.byte_192_extended_specification_compliance_codes)
            );
        }
    }

    void validateOptionValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Table 6-22 Option Values (Page 00h Bytes 193-195)
        if(programming.byte_193_option_values.reserved_bit_7) {
            validationResult.errors.push_back(
                fmt::format("Byte 193 (\"Option Values\") reserved bit 7 is set", programming.byte_192_extended_specification_compliance_codes)
            );
        }
    }

    void validateVendorSerialNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.25 Vendor Serial Number (00h 196-211)
        bool vendorSNAllZeros = std::all_of(programming.byte_196_211_vendor_sn.begin(), programming.byte_196_211_vendor_sn.end(), [](unsigned char val) { return val == 0; });
        if(!vendorSNAllZeros) {
            for(int index = 0; index < programming.byte_196_211_vendor_sn.size(); ++index) {
                if(!std::isprint(programming.byte_196_211_vendor_sn[index])) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte {} (\"Vendor SN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                            196 + index, index, programming.byte_196_211_vendor_sn[index]
                        )
                    );
                }
            }
        }
        //TODO: Warn if Vendor SN field is not left aligned, padded with spaces (0x20)?
    }

    void validateDateCode(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Date Code (00h 212-219)
        if(
            !std::isdigit(programming.byte_212_219_date_code.year_low_order_digits[0]) || 
            !std::isdigit(programming.byte_212_219_date_code.year_low_order_digits[1])
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 212 / 213 (\"Date Code\", low order digits of the year) is not a number"
                )
            );
        }
        //No need to parse: We checked that both characters are numbers [0-9] and all values are valid (year 2000 - 2099)

        if(
            programming.byte_212_219_date_code.month_digits[0] <= 0x2F || programming.byte_212_219_date_code.month_digits[0] >= 0x3A ||
            programming.byte_212_219_date_code.month_digits[1] <= 0x2F || programming.byte_212_219_date_code.month_digits[1] >= 0x3A
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 213 / 214 (\"Date Code\", digits of the month) is not a number"
                )
            );
        } else {
            try {
                int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.month_digits.data()), 2));
                if(digits_of_month < 1 || digits_of_month > 12) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte 213 / 214 (\"Date Code\", digits of the month) must be within [0;12] but is {}",
                            digits_of_month
                        )
                    );
                }
            } catch(const std::exception& e) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 213 / 214 (\"Date Code\", digits of the month) is not a valid number (could not parse?)"
                    )
                );
            }
        }

        if(
            programming.byte_212_219_date_code.day_digits[0] <= 0x2F || programming.byte_212_219_date_code.day_digits[0] >= 0x3A ||
            programming.byte_212_219_date_code.day_digits[1] <= 0x2F || programming.byte_212_219_date_code.day_digits[1] >= 0x3A
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 215 / 216 (\"Date Code\", day of the month) is not a number"
                )
            );
        } else {
            try {
                int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.day_digits.data()), 2));
                if(digits_of_month < 1 || digits_of_month > 31) {
                    validationResult.errors.push_back(
                        fmt::format(
                            "Byte 215 / 216 (\"Date Code\", day of the month) must be within [0;31] but is {}",
                            digits_of_month
                        )
                    );
                }
            } catch(const std::exception& e) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte 215 / 216 (\"Date Code\", day of the month) is not a valid number (could not parse?)"
                    )
                );
            }
        }

        for(int index = 0; index < programming.byte_212_219_date_code.lot_code.size(); ++index) {
            if(!std::isprint(programming.byte_212_219_date_code.lot_code[index])) {
                validationResult.errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor Lot Code\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        218 + index, index, programming.byte_212_219_date_code.lot_code[index]
                    )
                );
            }
        }
    }

    void validateDiagnosticMonitoringType(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.3.27 Diagnostic Monitoring Type (00h 220)
        if(programming.byte_220_diagnostic_monitoring_type.reserved_bit_7 || programming.byte_220_diagnostic_monitoring_type.reserved_bit_6 ||
            programming.byte_220_diagnostic_monitoring_type.reserved_bit_1 || programming.byte_220_diagnostic_monitoring_type.reserved_bit_0
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 220 (\"Diagnostic Monitoring Type\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                    programming.byte_220_diagnostic_monitoring_type.reserved_bit_7, programming.byte_220_diagnostic_monitoring_type.reserved_bit_6,
                    programming.byte_220_diagnostic_monitoring_type.reserved_bit_1, programming.byte_220_diagnostic_monitoring_type.reserved_bit_0
                )
            );
        }
    }

    void validateEnhancedOptions(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        if(programming.byte_221_enhanced_options.reserved_bit_7 || programming.byte_221_enhanced_options.reserved_bit_6 ||
            programming.byte_221_enhanced_options.reserved_bit_5 || programming.byte_221_enhanced_options.reserved_bit_2
        ) {
            validationResult.errors.push_back(
                fmt::format(
                    "Byte 221 (\"Enhanced Options\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 2 {:d}",
                    programming.byte_221_enhanced_options.reserved_bit_7, programming.byte_221_enhanced_options.reserved_bit_6,
                    programming.byte_221_enhanced_options.reserved_bit_5, programming.byte_221_enhanced_options.reserved_bit_2
                )
            );
        }
    }

    void validateExtendedBaudRate(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
        if(programming.byte_222_extended_baud_rate_in_250_mbaud != 0 && programming.byte_140_nominal_signaling_rate_in_100_mbaud != 0xFF) {
            validationResult.warnings.push_back(
                fmt::format("Byte 222 (\"Extended Baud Rate: Nominal\") is nonzero, but byte 140 (\"Nominal Signaling Rate\") is not 0xFF")
            );
        }
    }

    void validateRateSelectionConsistency(const SFF8636_Upper00h& programming, ValidationResult& validationResult) {
        //SFF-8636 Rev 2.11 Section 6.2.7
        //The free side device shall implement one of two options:
        //a) Provide no support for rate selection
        //b) Rate selection using extended rate select

        //6.2.7.1 No Rate Selection Support
        //When no rate selection is supported, (Page 00h Byte 221 bits 2 and 3) have a value of 0 and Options (Page 00h Byte 195 bit 5) has a value of 0

        //However, it is not specified what is supposed to happen with byte 141 Extended Rate Select Compliance
        //We assume that the "Reserved" value 00b shall be used if there is no rate selection support

        //6.2.7.2 Extended Rate Selection
        //When Page 00h Byte 195 bit 5 is 1 and Rate Select declaration bits (Page 00h Byte 221 bits 2 and 3) have the
        //values of 0 and 1 respectively and at least one of the bits in the Extended Rate Compliance byte (Page 00h Byte
        //141) has a value of one, the free side device supports extended rate select.

        bool noSupportForRateSelectionConsistent = 
            programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_OR_NOT_IMPLEMENTED_0b00 &&
            programming.byte_195_option_values.rate_select_implemented_bit_5 == false &&
            programming.byte_221_enhanced_options.reserved_bit_2 == false &&
            programming.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3 == false;

        bool supportForExtendedRateSelectionConsistent = 
            programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 != Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_OR_NOT_IMPLEMENTED_0b00 &&
            programming.byte_195_option_values.rate_select_implemented_bit_5 == true &&
            programming.byte_221_enhanced_options.reserved_bit_2 == false &&
            programming.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3 == true;

        if(!noSupportForRateSelectionConsistent && !supportForExtendedRateSelectionConsistent) {
            validationResult.errors.push_back(
                fmt::format(
                    "No consistent options for rate selection support - No Support for rate selection requires byte 141, bits 1-0 = 0b00, byte 195 bit 5 = 0, byte 221 bit 2 = 0, byte 221 bit 3 = 0 - "
                    "Rate selection using extended rate select requires byte 141, bits 1-0 != 0b00, byte 195 bit 5 = 1, byte 221 bit 2 = 0, byte 221 bit 3 = 1 - "
                    "Actual Settings: Byte 141 bits 1-0: {:#04b}, byte 195 bit 5 = {:d}, byte 221 bit 2 = {:d}, byte 221 bit 3 = {:d}",
                    getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0).bitValue,
                    programming.byte_195_option_values.rate_select_implemented_bit_5,
                    programming.byte_221_enhanced_options.reserved_bit_2,
                    programming.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3
                )
            );
        }

    }

    //TODO: Introduce options to not warn on values in "Vendor Specific" ranges (in case this tool is used by actual vendor?)
    ValidationResult validateSFF8636_Upper00h(const TransceiverTool::Standards::SFF8636::SFF8636_Upper00h& programming) {
        ValidationResult validationResult;

        //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
        validateIdentifierValues(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129)
        validateExtendedIdentifierValues(programming, validationResult);

        //SFF-8024 Rev 4.11 Table 4-3 Connector Types
        validateConnectorTypes(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
        validateSpecificationComplianceCodes(programming, validationResult);
        
        //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
        validateEncodingValues(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.6 Nominal Signaling Rate (00h 140) & Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
        validateNominalSignalingRate(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
        validateExtendedRateSelectCompliance(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.14 Vendor Name (00h 148-163)
        validateVendorName(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-21 Extended Module Code Values (Page 00h Byte 164)
        validateExtendedModuleCodes(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.17 Vendor Part Number (00h 168-183)
        validateVendorPartNumber(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.18 Vendor Revision Number (00h 184-185)
        validateVendorRevisionNumber(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.21 Maximum Case Temperature (00h 190)
        validateMaximumCaseTemperature(programming, validationResult);

        //FIXME: Verify byte 191 CC_Base checksum

        //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
        validateExtendedSpecificationComplianceCodes(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-22 Option Values (Page 00h Bytes 193-195)
        validateOptionValues(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.25 Vendor Serial Number (00h 196-211)
        validateVendorSerialNumber(programming, validationResult);

        //SFF-8636 Rev 2.11 Date Code (00h 212-219)
        validateDateCode(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.27 Diagnostic Monitoring Type (00h 220)
        validateDiagnosticMonitoringType(programming, validationResult);

        //SFF-8636 Rev 2.11 Section 6.3.28 Enhanced Options (00h 221)
        validateEnhancedOptions(programming, validationResult);

        //SFF-8636 Rev 2.11 Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
        validateExtendedBaudRate(programming, validationResult);

        //FIXME: Validate CC_EXT

        validateRateSelectionConsistency(programming, validationResult);

        return validationResult;
    }
}