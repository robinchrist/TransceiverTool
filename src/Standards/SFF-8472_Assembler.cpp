#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include <cstring>
#include <stdexcept>

namespace TransceiverTool::Standards::SFF8472 {


    //Creates the first 128 bytes of page A0h SFF-8472
    void assembleToBinary(unsigned char * target, const SFF8472_LowerA0h& programming, common::ChecksumDirective CC_BASEDirective, common::ChecksumDirective CC_EXTDirective) {

        target[0] = programming.byte_0_Identifier;

        target[1] = programming.byte_1_extended_identifier;

        target[2] = programming.byte_2_Connector_type;

        target[3] = 
            (unsigned char)(programming.byte_3_ethernet_compliance_codes._10GBASE_ER_bit_7) << 7 |
            (unsigned char)(programming.byte_3_ethernet_compliance_codes._10GBASE_LRM_bit_6) << 6 |
            (unsigned char)(programming.byte_3_ethernet_compliance_codes._10GBASE_LR_bit_5) << 5 |
            (unsigned char)(programming.byte_3_ethernet_compliance_codes._10GBASE_SR_bit_4) << 4 |
            (unsigned char)(programming.byte_3_infiniband_compliance_codes._1X_SX_bit_3) << 3 |
            (unsigned char)(programming.byte_3_infiniband_compliance_codes._1X_LX_bit_2) << 2 |
            (unsigned char)(programming.byte_3_infiniband_compliance_codes._1X_Copper_Active_bit_1) << 1 |
            (unsigned char)(programming.byte_3_infiniband_compliance_codes._1X_Copper_Passive_bit_0) << 0;


        target[4] = 
            (unsigned char)(programming.byte_4_escon_compliance_codes.ESCON_MMF_1310nm_LED_bit_7) << 7 |
            (unsigned char)(programming.byte_4_escon_compliance_codes.ESCON_SMF_1310nm_Laser_bit_6) << 6 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_192_short_reach_byte_4_bit_5) << 5 |
            getSFF8472_SONETReachSpecifierInfo(programming.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3).byte_value << 3 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_48_long_reach_byte_4_bit_2) << 2 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_48_intermediate_reach_byte_4_bit_1) << 1 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_48_short_reach_byte_4_bit_0) << 0;
        
        target[5] = 
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7) << 7 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_long_reach_2_byte_5_bit_6) << 6 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_intermediate_reach_byte_5_bit_5) << 5 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_12_short_reach_byte_5_bit_4) << 4 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3) << 3 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_long_reach_byte_5_bit_2) << 2 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_intermediate_reach_byte_5_bit_1) << 1 |
            (unsigned char)(programming.byte_4_5_sonet_compliance_codes.OC_3_short_reach__byte_5_bit_0) << 0;


        target[6] = 
            (unsigned char)(programming.byte_6_ethernet_compliance_codes.BASE_PX_bit_7) << 7 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes.BASE_BX10_bit_6) << 6 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._100BASE_FX_bit_5) << 5 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._100BASE_LX_LX10_bit_4) << 4 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._1000BASE_T_bit_3) << 3 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._1000BASE_CX_bit_2) << 2 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._1000BASE_LX_bit_1) << 1 |
            (unsigned char)(programming.byte_6_ethernet_compliance_codes._1000BASE_SX_bit_0) << 0;


        target[7] = 
            (unsigned char)(programming.byte_7_fibre_channel_link_length_codes.very_long_distance_v_bit_7) << 7 |
            (unsigned char)(programming.byte_7_fibre_channel_link_length_codes.short_distance_s_bit_6) << 6 |
            (unsigned char)(programming.byte_7_fibre_channel_link_length_codes.intermediate_distance_i_bit_5) << 5 |
            (unsigned char)(programming.byte_7_fibre_channel_link_length_codes.long_distance_l_bit_4) << 4 |
            (unsigned char)(programming.byte_7_fibre_channel_link_length_codes.medium_distance_m_bit_3) << 3 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_linear_Rx_SA_byte_7_bit_2) << 2 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Longwave_laser_LC_byte_7_bit_1) << 1 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Electrical_inter_enclosure_EL_byte_7_bit_0) << 0;


        target[8] = 
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Electrical_intra_enclosure_EL_byte_byte_8_bit_7) << 7 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6) << 6 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_with_OFC_SL_byte_8_bit_5) << 5 |
            (unsigned char)(programming.byte_7_8_fibre_channel_technology_codes.Longwave_Laser_LL_byte_8_bit_4) << 4 |
            (unsigned char)(programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) << 3 |
            (unsigned char)(programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2) << 2 |
            (unsigned char)(programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_0) << 0;


        target[9] = 
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Twin_Axial_Pair_TW_bit_7) << 7 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Twisted_Pair_TP_bit_6) << 6 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Miniature_Coax_MI_bit_5) << 5 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Video_Coax_TV_bit_4) << 4 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Multimode_62_5_um_M6_bit_3) << 3 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Multimode_50_um_M5_M5E_bit_2) << 2 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_9_fibre_channel_transmission_media_codes.Single_Mode_SM_bit_0) << 0;


        target[10] = 
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._1200_MBytes_sec_bit_7) << 7 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._800_MBytes_sec_bit_6) << 6 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._1600_MBytes_sec_bit_5) << 5 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._400_MBytes_sec_bit_4) << 4 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._3200_MBytes_sec_bit_3) << 3 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._200_MBytes_sec_bit_2) << 2 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes.see_byte_62_fibre_channel_speed_2_bit_1) << 1 |
            (unsigned char)(programming.byte_10_fibre_channel_speed_codes._100_MBytes_sec_bit_0) << 0;


        target[11] = programming.byte_11_Encoding;

        target[12] = programming.byte_12_nominal_signaling_rate_in_100_mbaud;

        target[13] = programming.byte_13_rate_identifier;

        target[14] = programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz;

        target[15] = programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz;

        target[16] = programming.byte_16_length_om2_in_10_m;

        target[17] = programming.byte_17_length_om1_in_10_m;

        target[18] = programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m;

        target[19] = programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value;

        std::memcpy(target + 20, programming.byte_20_35_vendor_name.data(), 16);

        target[36] = programming.byte_36_extended_specification_compliance_codes;

        std::memcpy(target + 37, programming.byte_37_39_vendor_oui.data(), 3);

        std::memcpy(target + 40, programming.byte_40_55_vendor_pn.data(), 16);

        std::memcpy(target + 56, programming.byte_56_59_vendor_rev.data(), 4);


        target[62] = 
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_5) << 5 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_4) << 4 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_3) << 3 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_2) << 2 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_62_fibre_channel_2_speed_codes._64_GFC_bit_0) << 0;


        target[66] = programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud;

        target[67] = programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;
        
    }
}