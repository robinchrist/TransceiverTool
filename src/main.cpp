#include <cstddef>
#include <exception>
#include <variant>
#include <string>
#include <array>
#include <vector>
#include <fmt/core.h>
#include <algorithm>

#include "TransceiverTool/Standards/SFF-8024_Transceiver_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8636_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8024_Encoding_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Rate_Select_Compliance.hpp"
#include "TransceiverTool/Standards/SFF-8636_Device_And_Transmitter_Technology.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Module_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8024_Extended_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8636_Option_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Date_Code.hpp"
#include "TransceiverTool/Standards/SFF-8636_Diagnostic_Monitoring_Type.hpp"
#include "TransceiverTool/Standards/SFF-8636_Enhanced_Options.hpp"

namespace TransceiverTool::Standards::SFF8636 {
    const Extended_Identifier_Bit_7_6_string& getSFF8636_Extended_Identifier_Bit_7_6Info(Extended_Identifier_Bit_7_6 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_7_6_strings.begin(),
            Extended_Identifier_Bit_7_6_strings.end(),
            [enum_value](const Extended_Identifier_Bit_7_6_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    const Extended_Identifier_Bit_1_0_string& getSFF8636_Extended_Identifier_Bit_1_0Info(Extended_Identifier_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Identifier_Bit_1_0_strings.begin(),
            Extended_Identifier_Bit_1_0_strings.end(),
            [enum_value](const Extended_Identifier_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }

    const Extended_Rate_Select_Compliance_Bit_1_0_string& getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(Extended_Rate_Select_Compliance_Bit_1_0 enum_value) {
        return *std::find_if(
            Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
            Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
            [enum_value](const Extended_Rate_Select_Compliance_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
        );
    }
}


struct SFF8636_Upper00h {

    //Byte 128: Identifier
    //Identifier Type of free side device (See SFF-8024 Transceiver Management)
    unsigned char byte_128_Identifier;

    //Byte 129: Extended Identifier (Ext. Identifier in spec)
    //Extended Identifier of free side device. Includes power classes, CLEI codes, CDR capability (See Table 6-16)
    TransceiverTool::Standards::SFF8636::Extended_Identifier byte_129_extended_identifier;


    //Byte 130: Connector Type
    //Code for media connector type (See SFF-8024 Transceiver Management)
    unsigned char byte_130_Connector_Type;

    //Bytes 131-138: Specification Compliance
    //Code for electronic or optical compatibility (See Table 6-17)

    //Byte 131: 10/40G/100G Ethernet Compliance Codes
    TransceiverTool::Standards::SFF8636::_10_40G_100G_Ethernet_Compliance_Codes byte_131_ethernet_compliance_codes;
    
    //Byte 132: SONET Compliance Codes
    TransceiverTool::Standards::SFF8636::SONET_Compliance_Codes byte_132_sonet_compliance_codes;

    //Byte 133: SAS/SATA Compliance Codes
    TransceiverTool::Standards::SFF8636::SAS_SATA_Compliance_Codes byte_133_sas_sata_compliance_codes;

    //Byte 134: Gigabit Ethernet Compliance Codes
    TransceiverTool::Standards::SFF8636::Gigabit_Ethernet_Compliance_Codes byte_134_gigabit_ethernet_compliance_codes;

    //Byte 135: Fibre Channel Link Length (7-3) & Fibre Channel Transmitter Technology (2-0)
    TransceiverTool::Standards::SFF8636::Fibre_Channel_Link_Length_And_Transmitter_Technology byte_135_fibre_channel_link_length_and_transmitter_technology;

    //Byte 136: Fibre Channel Transmitter Technology
    TransceiverTool::Standards::SFF8636::Fibre_Channel_Transmitter_Technology byte_136_fibre_channel_transmitter_technology;

    //Byte 137: Fibre Channel Transmission Media
    TransceiverTool::Standards::SFF8636::Fibre_Channel_Transmission_Media byte_137_fibre_channel_transmission_media;

    //Byte 138: Fibre Channel Speed
    TransceiverTool::Standards::SFF8636::Fibre_Channel_Speed byte_138_fibre_channel_speed;

    //Byte 139: Encoding
    //Code for serial encoding algorithm. (See SFF-8024 Transceiver Management)
    unsigned char byte_139_Encoding;

    //Byte 140: Nominal Signaling Rate
    //Nominal signaling rate, units of 100 MBd. For rate > 25.4 GBd, set this to FFh and use Byte 222
    unsigned char byte_140_nominal_signaling_rate_in_100_mbaud;

    //Byte 141: Extended Rate Select Compliance
    //Tags for extended rate select compliance. See Table 6-18.
    TransceiverTool::Standards::SFF8636::Extended_Rate_Select_Compliance byte_141_extended_rate_select_compliance;

    //Byte 142: Length (SMF)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for SMF fiber in km. A value of 1 shall be used for reaches from 0 to 1 km.
    unsigned char byte_142_length_smf_in_kilometers;

    //Byte 143: Length (OM3 50 um)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for EBW 50/125 um fiber (OM3), units of 2 m
    unsigned char byte_143_length_om3_in_2m;

    //Byte 144: Length (OM2 50 um)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 50/125 um fiber (OM2), units of 1 m
    unsigned char byte_144_length_om4_in_m;

    //Byte 145: Length (OM1 62.5 um) or Copper Cable Attenuation
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 62.5/125 um fiber (OM1), units of 1 m, 
    //or copper cable attenuation in dB at 25.78 GHz.
    unsigned char byte_145_length_om1_in_1m_or_copper_cable_attentuation_in_dB;


    //Byte 146: Length (passive copper or active cable or OM4 50 um)
    //Length of passive or active cable assembly (units of 1 m) or link length supported at the signaling rate in byte 140 or page 00h byte 222,
    //for OM4 50/125 um fiber (units of 2 m) as indicated by Byte 147. See 6.3.12
    unsigned char byte_146_length_copper_in_1m_or_om4_in_2m;

    //Byte 147: Device technology
    //Device technology (Table 6-19 and Table 6-20).
    TransceiverTool::Standards::SFF8636::Device_Technology_and_Transmitter_Technology byte_147_device_technology_and_transmitter_technology;

    //Byte 148-163: Vendor name
    //Free side device vendor name (ASCII)
    std::array<unsigned char, 16> byte_148_163_vendor_name;

    //Byte 164: Extended Module
    //Extended Module codes for InfiniBand (See Table 6-21)
    TransceiverTool::Standards::SFF8636::Extended_Module_Codes byte_164_extended_module_codes;

    //Byte 165-167: Vendor OUI
    //Free side device vendor IEEE company ID
    std::array<unsigned char, 3> byte_165_167_vendor_oui;

    //Byte 168-183: Vendor PN
    //Part number provided by free side device vendor(ASCII)
    std::array<unsigned char, 16> byte_168_183_vendor_pn;

    //Byte 184-185: Vendor rev
    //Revision level for part number provided by the vendor(ASCII)
    std::array<unsigned char, 2> byte_184_185_vendor_rev;

    //Byte 186-187: Wavelength or Copper Cable Attenuation
    //Nominal laser wavelength (wavelength=value/20 in nm) or copper cable attenuation in dB at 2.5 GHz (Byte 186) and 5.0 GHz (Byte 187)
    unsigned char byte_186_wavelength_high_order_or_copper_attentuation;
    unsigned char byte_187_wavelength_low_order_or_copper_attentuation;

    //Byte 188-189: Wavelength tolerance or Copper Cable Attenuation
    //The range of laser wavelength (+/- value) from nominal wavelength. (wavelength Tol. =value/200 in nm)
    //or copper cable attenuation in dB at 7.0 GHz (Byte 188) and 12.9 GHz (Byte 189)
    unsigned char byte_188_wavelength_tolerance_high_order_or_copper_attentuation;
    unsigned char byte_189_wavelength_tolerance_low_order_or_copper_attentuation;

    //Byte 190: Max case temp.
    //Maximum case temperature
    unsigned char byte_190_max_case_temperature;

    //Byte 191: CC_BASE
    //Check code for base ID fields (Bytes 128-190)
    unsigned char byte_191_CC_BASE;

    //Byte 192: Link codes
    //Extended Specification Compliance Codes (See SFF8024)
    unsigned char byte_192_extended_specification_compliance_codes;

    //Byte 193-195: Options
    //Optional features implemented. See Table 6-22.
    TransceiverTool::Standards::SFF8636::Option_Values_Byte_193 byte_193_option_values;
    TransceiverTool::Standards::SFF8636::Option_Values_Byte_194 byte_194_option_values;
    TransceiverTool::Standards::SFF8636::Option_Values_Byte_195 byte_195_option_values;

    //Byte 196-211: Vendor SN
    //Serial number provided by vendor (ASCII)
    std::array<unsigned char, 16> byte_196_211_vendor_sn;

    //Byte 212-219: Date Code
    //Vendor's manufacturing date code
    TransceiverTool::Standards::SFF8636::DateCode byte_212_219_date_code;

    //Byte 220: Diagnostic Monitoring Type
    //Indicates which type of diagnostic monitoring is implemented (if any) in the free side device.
    //Bit 1,0 Reserved. See Table 6-24
    TransceiverTool::Standards::SFF8636::Diagnostic_Monitoring_Type byte_220_diagnostic_monitoring_type;

    //Byte 221: Enhanced Options
    //Indicates which optional enhanced features are implemented in the free side device. See Table 6-24
    TransceiverTool::Standards::SFF8636::Enhanced_Options byte_221_enhanced_options;

    //Byte 222: Extended Baud Rate: Nominal
    //Nominal baud rate per channel, units of 250 MBd. Complements Byte 140. See Table 6-26.
    unsigned char byte_222_extended_baud_rate_in_250_mbaud;

    //Byte 223: CC_EXT
    //Check code for the Extended ID Fields (Bytes 192-222)
    unsigned char byte_223_CC_EXT;

    //Byte 224-255: Vendor Specific
    //Vendor Specific EEPROM
    std::array<unsigned char, 32> byte_224_255_vendor_specific;
};

struct ValidationResult {
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

//TODO: Introduce options to not warn on values in "Vendor Specific" ranges (in case this tool is used by actual vendor?)
ValidationResult validateSFF8636_Upper00h(const SFF8636_Upper00h& programming) {

    std::vector<std::string> warnings;
    std::vector<std::string> errors;

    //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
    if(programming.byte_128_Identifier >= 0x26 && programming.byte_128_Identifier <= 0x7F) {
        errors.push_back(
            fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
        );
    }
    if(programming.byte_128_Identifier >= 0x80) {
        warnings.push_back(
            fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Vendor Specific\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
        );
    }

    //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129)
    //If bit 1-0 is not 00 (i.e. Power Class 5/6/7), bit 7-6 must indicate 11 (Power Class 4-7)
    if(programming.byte_129_extended_identifier.power_class_bit_1_0 != TransceiverTool::Standards::SFF8636::Extended_Identifier_Bit_1_0::POWER_CLASSES_1_TO_4_0b00 &&
        programming.byte_129_extended_identifier.power_class_bit_7_6 != TransceiverTool::Standards::SFF8636::Extended_Identifier_Bit_7_6::POWER_CLASS_4_TO_7_0b11
    ) {
        errors.push_back(
            //TODO: add bits value present in programming to error message?
            fmt::format("Byte 129 (\"Extended Identifier\") inconsistent values: value bits 1-0 indicate power class \"{}\", but bits 7-6 indicates \"{}\"", 
                getSFF8636_Extended_Identifier_Bit_1_0Info(programming.byte_129_extended_identifier.power_class_bit_1_0).description,
                getSFF8636_Extended_Identifier_Bit_7_6Info(programming.byte_129_extended_identifier.power_class_bit_7_6).description
            )
        );
    }


    //SFF-8024 Rev 4.11 Table 4-3 Connector Types
    if(
        (programming.byte_130_Connector_Type >= 0x0E && programming.byte_130_Connector_Type <= 0x1F) ||
        (programming.byte_130_Connector_Type >= 0x29 && programming.byte_130_Connector_Type <= 0x7F)
    ) {
        errors.push_back(
            fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
        );
    }
    if(programming.byte_130_Connector_Type >= 0x80) {
        warnings.push_back(
            fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Vendor specific\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
        );
    }

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    //Byte 132: SONET Compliance Codes
    if(programming.byte_132_sonet_compliance_codes.reserved_bit_7 || programming.byte_132_sonet_compliance_codes.reserved_bit_6 ||
        programming.byte_132_sonet_compliance_codes.reserved_bit_5 || programming.byte_132_sonet_compliance_codes.reserved_bit_4 ||
        programming.byte_132_sonet_compliance_codes.reserved_bit_3
    ) {
        errors.push_back(
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
        errors.push_back(
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
        errors.push_back(
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
        errors.push_back(
            fmt::format(
                "Byte 136 (\"Fibre Channel Transmitter Technology\") value has at least one reserved bit set: Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2, 
                programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0
            )
        );
    }

    //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
    if(programming.byte_139_Encoding >= 0x09) {
        errors.push_back(
            fmt::format("Byte 139 (\"Encoding\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-2 \"Encoding Values\"), value is {:#04x}", programming.byte_139_Encoding)
        );
    }

    //SFF-8636 Rev 2.11 Section 6.3.6 Nominal Signaling Rate (00h 140) & Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0) {
        warnings.push_back(
            fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0")
        );
    }
    if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0xFF) {
        if(programming.byte_222_extended_baud_rate_in_250_mbaud == 0) {
            warnings.push_back(
            fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but Byte 222 (\"Extended Baud Rate: Nominal\") is zero")
            );
        } else {
            unsigned long nominalBaudRateFromByte222 = (unsigned long)(programming.byte_222_extended_baud_rate_in_250_mbaud) * 250UL;

            if(nominalBaudRateFromByte222 < 25400) {
                warnings.push_back(
                fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but value from Byte 222 (\"Extended Baud Rate: Nominal\") is too small ({} MBd)", nominalBaudRateFromByte222)
                );
            }
        }
    }

    //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
    if(programming.byte_141_extended_rate_select_compliance.reserved_bit_7 || programming.byte_141_extended_rate_select_compliance.reserved_bit_6 ||
        programming.byte_141_extended_rate_select_compliance.reserved_bit_5 || programming.byte_141_extended_rate_select_compliance.reserved_bit_4 ||
        programming.byte_141_extended_rate_select_compliance.reserved_bit_3 || programming.byte_141_extended_rate_select_compliance.reserved_bit_2 ||
        programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == TransceiverTool::Standards::SFF8636::Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b00 ||
        programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == TransceiverTool::Standards::SFF8636::Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b11
    ) {
        errors.push_back(
            fmt::format(
                "Byte 132 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1-0: \"{}\"",
                programming.byte_141_extended_rate_select_compliance.reserved_bit_7, programming.byte_141_extended_rate_select_compliance.reserved_bit_6,
                programming.byte_141_extended_rate_select_compliance.reserved_bit_5, programming.byte_141_extended_rate_select_compliance.reserved_bit_4,
                programming.byte_141_extended_rate_select_compliance.reserved_bit_3, programming.byte_141_extended_rate_select_compliance.reserved_bit_2,
                TransceiverTool::Standards::SFF8636::getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0)
            )
        );
    }

    //SFF-8636 Rev 2.11 Section 6.3.14 Vendor Name (00h 148-163)
    for(int index = 0; index < programming.byte_148_163_vendor_name.size(); ++index) {
        if(programming.byte_148_163_vendor_name[index] <= 0x19 || programming.byte_148_163_vendor_name[index] >= 0x7F) {
            errors.push_back(
                fmt::format(
                    "Byte {} (\"Vendor Name\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                    148 + index, index, programming.byte_148_163_vendor_name[index]
                )
            );
        }
    }
    //TODO: Warn if Vendor Name field is not left aligned, padded with spaces (0x20)?

    //SFF-8636 Rev 2.11 Table 6-21 Extended Module Code Values (Page 00h Byte 164)
    if(programming.byte_164_extended_module_codes.reserved_bit_7 || programming.byte_164_extended_module_codes.reserved_bit_6) {
        errors.push_back(
            fmt::format(
                "Byte 164 (\"Extended Module Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}",
                programming.byte_164_extended_module_codes.reserved_bit_7, programming.byte_164_extended_module_codes.reserved_bit_6
            )
        );
    }

    //SFF-8636 Rev 2.11 Section 6.3.17 Vendor Part Number (00h 168-183)
    bool vendorPNAllZeros = std::all_of(programming.byte_168_183_vendor_pn.begin(), programming.byte_168_183_vendor_pn.end(), [](unsigned char val) { return val == 0; });
    if(!vendorPNAllZeros) {
        for(int index = 0; index < programming.byte_168_183_vendor_pn.size(); ++index) {
            if(programming.byte_168_183_vendor_pn[index] <= 0x19 || programming.byte_168_183_vendor_pn[index] >= 0x7F) {
                errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor PN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        168 + index, index, programming.byte_168_183_vendor_pn[index]
                    )
                );
            }
        }
    }
    //TODO: Warn if Vendor PN field is not left aligned, padded with spaces (0x20)?

    //SFF-8636 Rev 2.11 Section 6.3.18 Vendor Revision Number (00h 184-185
    bool vendorRevAllZeros = std::all_of(programming.byte_184_185_vendor_rev.begin(), programming.byte_184_185_vendor_rev.end(), [](unsigned char val) { return val == 0; });
    if(!vendorRevAllZeros) {
        for(int index = 0; index < programming.byte_184_185_vendor_rev.size(); ++index) {
            if(programming.byte_184_185_vendor_rev[index] <= 0x19 || programming.byte_184_185_vendor_rev[index] >= 0x7F) {
                errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor Rev\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        184 + index, index, programming.byte_184_185_vendor_rev[index]
                    )
                );
            }
        }
    }
    //TODO: Warn if Vendor Rev field is not left aligned, padded with spaces (0x20)?

    //SFF-8636 Rev 2.11 Section 6.3.21 Maximum Case Temperature (00h 190)
    if(programming.byte_190_max_case_temperature == 70) {
        warnings.push_back(
            fmt::format(
                "Byte 190 (\"Maximum Case Temperature\") value is {:#04x} -> 70 deg C, but standards mandates setting to 00h for 70 deg C and values other than 00h must only be used for maximum case temperatures that are not 70 deg C",
                programming.byte_190_max_case_temperature
            )
        );
    }

    //FIXME: Verify byte 191 CC_Base checksum

    //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
    if(programming.byte_192_extended_specification_compliance_codes == 0x0A || programming.byte_192_extended_specification_compliance_codes == 0x0F ||
        (programming.byte_192_extended_specification_compliance_codes >= 0x3B && programming.byte_192_extended_specification_compliance_codes <= 0x3E) ||
        (programming.byte_192_extended_specification_compliance_codes >= 0x4D && programming.byte_192_extended_specification_compliance_codes <= 0x7E) ||
        programming.byte_192_extended_specification_compliance_codes >= 0x82
    ) {
        errors.push_back(
            fmt::format("Byte 192 (\"Extended Specification Compliance Codes\") value corresponds to \"Reserved\" range / value (SFF-8024 Rev 4.11 Table 4-4 \"Extended Specification Compliance Codes\"), value is {:#04x}", programming.byte_192_extended_specification_compliance_codes)
        );
    }

    //SFF-8636 Rev 2.11 Table 6-22 Option Values (Page 00h Bytes 193-195)
    if(programming.byte_193_option_values.reserved_bit_7) {
        errors.push_back(
            fmt::format("Byte 193 (\"Option Values\") reserved bit 7 is set", programming.byte_192_extended_specification_compliance_codes)
        );
    }
    
    //SFF-8636 Rev 2.11 Section 6.3.25 Vendor Serial Number (00h 196-211)
    bool vendorSNAllZeros = std::all_of(programming.byte_196_211_vendor_sn.begin(), programming.byte_196_211_vendor_sn.end(), [](unsigned char val) { return val == 0; });
    if(!vendorSNAllZeros) {
        for(int index = 0; index < programming.byte_196_211_vendor_sn.size(); ++index) {
            if(programming.byte_196_211_vendor_sn[index] <= 0x19 || programming.byte_196_211_vendor_sn[index] >= 0x7F) {
                errors.push_back(
                    fmt::format(
                        "Byte {} (\"Vendor SN\", Position {}) is an unprintable ASCII character (Byte Value {:#04x})",
                        196 + index, index, programming.byte_196_211_vendor_sn[index]
                    )
                );
            }
        }
    }
    //TODO: Warn if Vendor SN field is not left aligned, padded with spaces (0x20)?


    //SFF-8636 Rev 2.11 Date Code (00h 212-219)
    if(
        programming.byte_212_219_date_code.year_low_order_digits[0] <= 0x2F || programming.byte_212_219_date_code.year_low_order_digits[0] >= 0x3A ||
        programming.byte_212_219_date_code.year_low_order_digits[1] <= 0x2F || programming.byte_212_219_date_code.year_low_order_digits[1] >= 0x3A
    ) {
        errors.push_back(
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
        errors.push_back(
            fmt::format(
                "Byte 213 / 214 (\"Date Code\", digits of the month) is not a number"
            )
        );
    } else {
        try {
            int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.month_digits.data()), 2));
            if(digits_of_month < 0 || digits_of_month > 12) {
                errors.push_back(
                    fmt::format(
                        "Byte 213 / 214 (\"Date Code\", digits of the month) must be within [0;12] but is {}",
                        digits_of_month
                    )
                );
            }
        } catch(const std::exception& e) {
            errors.push_back(
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
        errors.push_back(
            fmt::format(
                "Byte 215 / 216 (\"Date Code\", day of the month) is not a number"
            )
        );
    } else {
        try {
            int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.day_digits.data()), 2));
            if(digits_of_month < 0 || digits_of_month > 31) {
                errors.push_back(
                    fmt::format(
                        "Byte 215 / 216 (\"Date Code\", day of the month) must be within [0;31] but is {}",
                        digits_of_month
                    )
                );
            }
        } catch(const std::exception& e) {
            errors.push_back(
                fmt::format(
                    "Byte 215 / 216 (\"Date Code\", day of the month) is not a valid number (could not parse?)"
                )
            );
        }
    }

    //SFF-8636 Rev 2.11 Section 6.3.27 Diagnostic Monitoring Type (00h 220)
    if(programming.byte_220_diagnostic_monitoring_type.reserved_bit_7 || programming.byte_220_diagnostic_monitoring_type.reserved_bit_6 ||
        programming.byte_220_diagnostic_monitoring_type.reserved_bit_1 || programming.byte_220_diagnostic_monitoring_type.reserved_bit_0
    ) {
        errors.push_back(
            fmt::format(
                "Byte 220 (\"Diagnostic Monitoring Type\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                programming.byte_220_diagnostic_monitoring_type.reserved_bit_7, programming.byte_220_diagnostic_monitoring_type.reserved_bit_6,
                programming.byte_220_diagnostic_monitoring_type.reserved_bit_1, programming.byte_220_diagnostic_monitoring_type.reserved_bit_0
            )
        );
    }


    if(programming.byte_221_enhanced_options.reserved_bit_7 || programming.byte_221_enhanced_options.reserved_bit_6 ||
        programming.byte_221_enhanced_options.reserved_bit_5 || programming.byte_221_enhanced_options.reserved_bit_2
    ) {
        errors.push_back(
            fmt::format(
                "Byte 221 (\"Enhanced Options\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 2 {:d}",
                programming.byte_221_enhanced_options.reserved_bit_7, programming.byte_221_enhanced_options.reserved_bit_6,
                programming.byte_221_enhanced_options.reserved_bit_5, programming.byte_221_enhanced_options.reserved_bit_2
            )
        );
    }

    //SFF-8636 Rev 2.11 Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    if(programming.byte_222_extended_baud_rate_in_250_mbaud != 0 && programming.byte_140_nominal_signaling_rate_in_100_mbaud != 0xFF) {
        warnings.push_back(
            fmt::format("Byte 222 (\"Extended Baud Rate: Nominal\") is nonzero, but byte 140 (\"Nominal Signaling Rate\") is not 0xFF")
        );
    }

    //FIXME: Validate CC_EXT

    return ValidationResult {
        std::move(warnings),
        std::move(errors)
    };
}

int main() {
    
    return 0;
}