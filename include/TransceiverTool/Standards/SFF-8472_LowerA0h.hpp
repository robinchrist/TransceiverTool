#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Date_Code.hpp"
#include "TransceiverTool/Standards/SFF-8472_Diagnostic_Monitoring_Type.hpp"
#include "TransceiverTool/Standards/SFF-8472_Option_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Enhanced_Options.hpp"

namespace TransceiverTool::Standards::SFF8472 {
    struct SFF8472_LowerA0h {

        //Byte 0: Identifier
        //Type of transceiver (see Table 5-1)
        unsigned char byte_0_Identifier;

        //Byte 1: Extended Identifier
        //Extended identifier of type of transceiver (see Table 5-2)
        unsigned char byte_1_extended_identifier;

        //Byte 2: Connector
        //Code for connector type (see SFF-8024 SFF Module Management Reference Code Tables)
        unsigned char byte_2_Connector_type;

        //Byte 3: 10G Ethernet Compliance Code & Infiniband Compliance Codes
        _10G_Ethernet_Compliance_Codes byte_3_ethernet_compliance_codes;
        Infiniband_Compliance_Codes byte_3_infiniband_compliance_codes;

        //Byte 4: ESCON Compliance Codes
        ESCON_Compliance_Codes byte_4_escon_compliance_codes;

        //Byte 4 + 5: SONET Compliance Codes
        SONET_Compliance_Codes byte_4_5_sonet_compliance_codes;

        //Byte 6: Ethernet Compliance Codes
        Ethernet_Compliance_Codes byte_6_ethernet_compliance_codes;

        //Byte 7: Fibre Channel Link Length
        Fibre_Channel_Link_Length_Codes byte_7_fibre_channel_link_length_codes;

        //Byte 7+8: Fibre Channel Technology
        Fibre_Channel_Technology_Codes byte_7_8_fibre_channel_technology_codes;

        //Byte 8: SFP+ Cable Technology
        SFP_plus_Cable_Technology_Codes byte_8_sfp_plus_cable_technology_codes;

        //Byte 9: Fibre Channel Transmission Media
        Fibre_Channel_Transmission_Media_Codes byte_9_fibre_channel_transmission_media_codes;

        //Byte 10: Fibre Channel Speed
        Fibre_Channel_Speed_Codes byte_10_fibre_channel_speed_codes;

        //Byte 11: Encoding
        //Code for high speed serial encoding algorithm (see SFF-8024 SFF Module Management Reference Code Tables)
        unsigned char byte_11_Encoding;

        //Byte 12: Nominal Signaling Rate
        //Nominal signaling rate, units of 100 MBd. (see details for rates > 25.4 GBd)
        unsigned char byte_12_nominal_signaling_rate_in_100_mbaud;

        //Byte 13: Rate Identifier
        //Type of rate select functionality (see Table 5-6)
        unsigned char byte_13_rate_identifier;

        //Byte 14: Length (SMF,km) or Copper Cable Attenuation
        //Link length supported for single-mode fiber, units of km, or copper cable attenuation in dB at 12.9 GHz
        unsigned char byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz;

        //Byte 15: Length (SMF) or Copper Cable Attenuation
        //Link length supported for single-mode fiber, units of 100 m, or copper cable attenuation in dB at 25.78 GHz
        unsigned char byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz;

        //Byte 16: Length (50 um, OM2)
        //Link length supported for 50 um OM2 fiber, units of 10 m
        unsigned char byte_16_length_om2_in_10_m;

        //Byte 17: Length (62.5 um, OM1) 
        //Link length supported for 62.5 um OM1 fiber, units of 10 m
        unsigned char byte_17_length_om1_in_10_m;

        //Byte 18: Length (OM4 or copper cable)
        //Link length supported for 50um OM4 fiber, units of 10 m. Alternatively, copper or direct attach cable, units of m
        unsigned char byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m;

        //Byte 19: Length (OM3) or Cable length, additional
        //Link length supported for 50 um OM3 fiber, units of 10 m. Alternatively, copper or direct attach cable multiplier and base value
        unsigned char byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value;

        //Byte 20-35: Vendor name
        //SFP vendor name (ASCII)
        std::array<unsigned char, 16> byte_20_35_vendor_name;

        //Byte 36: Extended Specification Compliance Codes
        //Code for electronic or optical compatibility (see Table 5-3)
        //See SFF-8024 Table 4-4 
        unsigned char byte_36_extended_specification_compliance_codes;

        //Byte 37-39: Vendor OUI
        //SFP vendor IEEE company ID
        std::array<unsigned char, 3> byte_37_39_vendor_oui;

        //Byte 40-55: Vendor PN
        //Part number provided by SFP vendor (ASCII)
        std::array<unsigned char, 16> byte_40_55_vendor_pn;

        //Byte 56-59: Vendor Rev
        //Revision level for part number provided by vendor (ASCII)
        std::array<unsigned char, 4> byte_56_59_vendor_rev;

        //Byte 60-61: Wavelength
        //Laser wavelength (Passive/Active Cable Specification Compliance)
        unsigned char byte_60_wavelength_high_order_byte_or_cable_specification_compliance;
        unsigned char byte_61_wavelength_low_order_byte_or_cable_specification_compliance;

        //Byte 62: Fibre Channel Speed 2
        Fibre_Channel_Speed_2_Codes byte_62_fibre_channel_2_speed_codes;

        //Byte 63: CC_BASE
        //Check code for Base ID Fields (addresses 0 to 62)
        unsigned char byte_63_CC_BASE;

        Option_Values_Byte_64 byte_64_option_values;

        Option_Values_Byte_65 byte_65_option_values;

        //Byte 66: Signaling Rate, max 
        //Upper signaling rate margin, units of % (see details for rates >25.4 GBd)
        //If address 12 is not set to FFh, the upper signaling rate limit at which the transceiver still meets its specifications
        //(Signaling Rate, max) is specified in units of 1% above the nominal signaling rate.
        //If address 12 is set to FFh, the nominal signaling rate (Signaling Rate, nominal) is given in this field in units of 250 MBd
        unsigned char byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud;

        //Byte 67: Signaling rate, min
        //Lower signaling rate margin, units of % (see details for rates >25.4 GBd)
        //If address 12 is not set to FFh, the lower signaling rate limit at which the transceiver still meets its specifications
        //(Signaling Rate, min) is specified in units of 1% below the nominal bit rate.
        //If address 12 is set to FFh, the limit range of signaling rates specified in units of +/- 1% around the nominal signaling rate.
        unsigned char byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;

        //Byte 68-83: Vendor SN
        //Serial number provided by vendor (ASCII)
        std::array<unsigned char, 16> byte_68_83_vendor_sn;

        //Byte 84-91: Date Code
        //Vendor's manufacturing date code (see Table 8-4)
        DateCode byte_84_91_date_code;

        //Byte 92: Diagnostic Monitoring Type
        //Indicates which type of diagnostic monitoring is implemented
        //(if any) in the transceiver (see Table 8-5)
        Diagnostic_Monitoring_Type byte_92_diagnostic_monitoring_type;

        //Byte 93: Enhanced Options 
        //Indicates which optional enhanced features are implemented
        //(if any) in the transceiver (see Table 8-6)
        Enhanced_Options byte_93_enhanced_options;

        //Byte 94: SFF-8472 Compliance
        //Indicates which revision of SFF-8472 the transceiver complies with.
        //(see Table 8-8)
        unsigned char byte_94_sff_8472_compliance;

        //Byte 95: CC_EXT
        //Check code for the Extended ID Fields (addresses 64 to 94)
        unsigned char byte_95_CC_EXT;

        //Byte 96-127: Vendor Specific
        //Vendor Specific EEPROM
        std::array<unsigned char, 32> byte_96_127_vendor_specific;
    };
}