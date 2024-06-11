#pragma once
#include <string>
#include <array>
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
#include <nlohmann/json.hpp>

namespace TransceiverTool::Standards::SFF8636 {
    struct SFF8636_Upper00h {

        //Byte 128: Identifier
        //Identifier Type of free side device (See SFF-8024 Transceiver Management)
        unsigned char byte_128_Identifier;

        //Byte 129: Extended Identifier (Ext. Identifier in spec)
        //Extended Identifier of free side device. Includes power classes, CLEI codes, CDR capability (See Table 6-16)
        Extended_Identifier byte_129_extended_identifier;


        //Byte 130: Connector Type
        //Code for media connector type (See SFF-8024 Transceiver Management)
        unsigned char byte_130_Connector_Type;

        //Bytes 131-138: Specification Compliance
        //Code for electronic or optical compatibility (See Table 6-17)

        //Byte 131: 10/40G/100G Ethernet Compliance Codes
        _10_40G_100G_Ethernet_Compliance_Codes byte_131_ethernet_compliance_codes;
        
        //Byte 132: SONET Compliance Codes
        SONET_Compliance_Codes byte_132_sonet_compliance_codes;

        //Byte 133: SAS/SATA Compliance Codes
        SAS_SATA_Compliance_Codes byte_133_sas_sata_compliance_codes;

        //Byte 134: Gigabit Ethernet Compliance Codes
        Gigabit_Ethernet_Compliance_Codes byte_134_gigabit_ethernet_compliance_codes;

        //Byte 135: Fibre Channel Link Length (7-3) & Fibre Channel Transmitter Technology (2-0)
        Fibre_Channel_Link_Length_And_Transmitter_Technology byte_135_fibre_channel_link_length_and_transmitter_technology;

        //Byte 136: Fibre Channel Transmitter Technology
        Fibre_Channel_Transmitter_Technology byte_136_fibre_channel_transmitter_technology;

        //Byte 137: Fibre Channel Transmission Media
        Fibre_Channel_Transmission_Media byte_137_fibre_channel_transmission_media;

        //Byte 138: Fibre Channel Speed
        Fibre_Channel_Speed byte_138_fibre_channel_speed;

        //Byte 139: Encoding
        //Code for serial encoding algorithm. (See SFF-8024 Transceiver Management)
        unsigned char byte_139_Encoding;

        //Byte 140: Nominal Signaling Rate
        //Nominal signaling rate, units of 100 MBd. For rate > 25.4 GBd, set this to FFh and use Byte 222
        unsigned char byte_140_nominal_signaling_rate_in_100_mbaud;

        //Byte 141: Extended Rate Select Compliance
        //Tags for extended rate select compliance. See Table 6-18.
        Extended_Rate_Select_Compliance byte_141_extended_rate_select_compliance;

        //Byte 142: Length (SMF)
        //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for SMF fiber in km. A value of 1 shall be used for reaches from 0 to 1 km.
        unsigned char byte_142_length_smf_in_kilometers;

        //Byte 143: Length (OM3 50 um)
        //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for EBW 50/125 um fiber (OM3), units of 2 m
        unsigned char byte_143_length_om3_in_2m;

        //Byte 144: Length (OM2 50 um)
        //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 50/125 um fiber (OM2), units of 1 m
        unsigned char byte_144_length_om2_in_m;

        //Byte 145: Length (OM1 62.5 um) or Copper Cable Attenuation
        //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 62.5/125 um fiber (OM1), units of 1 m, 
        //or copper cable attenuation in dB at 25.78 GHz.
        unsigned char byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB;


        //Byte 146: Length (passive copper or active cable or OM4 50 um)
        //Length of passive or active cable assembly (units of 1 m) or link length supported at the signaling rate in byte 140 or page 00h byte 222,
        //for OM4 50/125 um fiber (units of 2 m) as indicated by Byte 147. See 6.3.12
        unsigned char byte_146_length_copper_in_1m_or_om4_in_2m;

        //Byte 147: Device technology
        //Device technology (Table 6-19 and Table 6-20).
        Device_Technology_and_Transmitter_Technology byte_147_device_technology_and_transmitter_technology;

        //Byte 148-163: Vendor name
        //Free side device vendor name (ASCII)
        std::array<unsigned char, 16> byte_148_163_vendor_name;

        //Byte 164: Extended Module
        //Extended Module codes for InfiniBand (See Table 6-21)
        Extended_Module_Codes byte_164_extended_module_codes;

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
        unsigned char byte_186_wavelength_high_order_or_copper_attenuation;
        unsigned char byte_187_wavelength_low_order_or_copper_attenuation;

        //Byte 188-189: Wavelength tolerance or Copper Cable Attenuation
        //The range of laser wavelength (+/- value) from nominal wavelength. (wavelength Tol. =value/200 in nm)
        //or copper cable attenuation in dB at 7.0 GHz (Byte 188) and 12.9 GHz (Byte 189)
        unsigned char byte_188_wavelength_tolerance_high_order_or_copper_attenuation;
        unsigned char byte_189_wavelength_tolerance_low_order_or_copper_attenuation;

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
        Option_Values_Byte_193 byte_193_option_values;
        Option_Values_Byte_194 byte_194_option_values;
        Option_Values_Byte_195 byte_195_option_values;

        //Byte 196-211: Vendor SN
        //Serial number provided by vendor (ASCII)
        std::array<unsigned char, 16> byte_196_211_vendor_sn;

        //Byte 212-219: Date Code
        //Vendor's manufacturing date code
        DateCode byte_212_219_date_code;

        //Byte 220: Diagnostic Monitoring Type
        //Indicates which type of diagnostic monitoring is implemented (if any) in the free side device.
        //Bit 1,0 Reserved. See Table 6-24
        Diagnostic_Monitoring_Type byte_220_diagnostic_monitoring_type;

        //Byte 221: Enhanced Options
        //Indicates which optional enhanced features are implemented in the free side device. See Table 6-24
        Enhanced_Options byte_221_enhanced_options;

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

    void SFF8636_Upper00hToJSON(nlohmann::ordered_json& j, const SFF8636_Upper00h& programming, bool copperMode);
    void SFF8636_Upper00hFromJSON(const nlohmann::json& j, SFF8636_Upper00h& programming);
}