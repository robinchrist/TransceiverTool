#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8472 {

    //Expects 256 bytes!
    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h parseBytesToStruct(unsigned char const * bytes) {
        SFF8472_LowerA0h parsedStruct;

        parsedStruct.byte_0_Identifier = bytes[0];

        parsedStruct.byte_1_extended_identifier = bytes[1];

        parsedStruct.byte_2_Connector_type = bytes[2];
        

        parsedStruct.byte_3_ethernet_compliance_codes._10GBASE_ER_bit_7 = bytes[3] & (1 << 7);
        parsedStruct.byte_3_ethernet_compliance_codes._10GBASE_LRM_bit_6 = bytes[3] & (1 << 6);
        parsedStruct.byte_3_ethernet_compliance_codes._10GBASE_LR_bit_5 = bytes[3] & (1 << 5);
        parsedStruct.byte_3_ethernet_compliance_codes._10GBASE_SR_bit_4 = bytes[3] & (1 << 4);
        parsedStruct.byte_3_infiniband_compliance_codes._1X_SX_bit_3 = bytes[3] & (1 << 3);
        parsedStruct.byte_3_infiniband_compliance_codes._1X_LX_bit_2 = bytes[3] & (1 << 2);
        parsedStruct.byte_3_infiniband_compliance_codes._1X_Copper_Active_bit_1 = bytes[3] & (1 << 1);
        parsedStruct.byte_3_infiniband_compliance_codes._1X_Copper_Passive_bit_0 = bytes[3] & (1 << 0);


        parsedStruct.byte_4_escon_compliance_codes.ESCON_MMF_1310nm_LED_bit_7 = bytes[4] & (1 << 7);
        parsedStruct.byte_4_escon_compliance_codes.ESCON_SMF_1310nm_Laser_bit_6 = bytes[4] & (1 << 6);

        parsedStruct.byte_4_5_sonet_compliance_codes.OC_192_short_reach_byte_4_bit_5 = bytes[4] & (1 << 5);
        parsedStruct.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3 = getSFF8472_SONETReachSpecifier_From_Char(bytes[4]);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_48_long_reach_byte_4_bit_2 = bytes[4] & (1 << 2);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_48_intermediate_reach_byte_4_bit_1 = bytes[4] & (1 << 1);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_48_short_reach_byte_4_bit_0 = bytes[4] & (1 << 0);
        parsedStruct.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7 = bytes[5] & (1 << 7);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_12_single_mode_long_reach_2_byte_5_bit_6 = bytes[5] & (1 << 6);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_12_single_mode_intermediate_reach_byte_5_bit_5 = bytes[5] & (1 << 5);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_12_short_reach_byte_5_bit_4 = bytes[5] & (1 << 4);
        parsedStruct.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3 = bytes[5] & (1 << 3);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_3_single_mode_long_reach_byte_5_bit_2 = bytes[5] & (1 << 2);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_3_single_mode_intermediate_reach_byte_5_bit_1 = bytes[5] & (1 << 1);
        parsedStruct.byte_4_5_sonet_compliance_codes.OC_3_short_reach__byte_5_bit_0 = bytes[5] & (1 << 0);


        parsedStruct.byte_6_ethernet_compliance_codes.BASE_PX_bit_7 = bytes[6] & (1 << 7);
        parsedStruct.byte_6_ethernet_compliance_codes.BASE_BX10_bit_6 = bytes[6] & (1 << 6);
        parsedStruct.byte_6_ethernet_compliance_codes._100BASE_FX_bit_5 = bytes[6] & (1 << 5);
        parsedStruct.byte_6_ethernet_compliance_codes._100BASE_LX_LX10_bit_4 = bytes[6] & (1 << 4);
        parsedStruct.byte_6_ethernet_compliance_codes._1000BASE_T_bit_3 = bytes[6] & (1 << 3);
        parsedStruct.byte_6_ethernet_compliance_codes._1000BASE_CX_bit_2 = bytes[6] & (1 << 2);
        parsedStruct.byte_6_ethernet_compliance_codes._1000BASE_LX_bit_1 = bytes[6] & (1 << 1);
        parsedStruct.byte_6_ethernet_compliance_codes._1000BASE_SX_bit_0 = bytes[6] & (1 << 0);


        parsedStruct.byte_7_fibre_channel_link_length_codes.very_long_distance_v_bit_7 = bytes[7] & (1 << 7);
        parsedStruct.byte_7_fibre_channel_link_length_codes.short_distance_s_bit_6 = bytes[7] & (1 << 6);
        parsedStruct.byte_7_fibre_channel_link_length_codes.intermediate_distance_i_bit_5 = bytes[7] & (1 << 5);
        parsedStruct.byte_7_fibre_channel_link_length_codes.long_distance_l_bit_4 = bytes[7] & (1 << 4);
        parsedStruct.byte_7_fibre_channel_link_length_codes.medium_distance_m_bit_3 = bytes[7] & (1 << 3);

        parsedStruct.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_linear_Rx_SA_byte_7_bit_2 = bytes[7] & (1 << 2);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Longwave_laser_LC_byte_7_bit_1 = bytes[7] & (1 << 1);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Electrical_inter_enclosure_EL_byte_7_bit_0 = bytes[7] & (1 << 0);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Electrical_intra_enclosure_EL_byte_byte_8_bit_7 = bytes[8] & (1 << 7);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6 = bytes[8] & (1 << 6);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_with_OFC_SL_byte_8_bit_5 = bytes[8] & (1 << 5);
        parsedStruct.byte_7_8_fibre_channel_technology_codes.Longwave_Laser_LL_byte_8_bit_4 = bytes[8] & (1 << 4);

        parsedStruct.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3 =  bytes[8] & (1 << 3);
        parsedStruct.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 =  bytes[8] & (1 << 2);
        parsedStruct.byte_8_sfp_plus_cable_technology_codes.reserved_bit_1 =  bytes[8] & (1 << 1);
        parsedStruct.byte_8_sfp_plus_cable_technology_codes.reserved_bit_0 =  bytes[8] & (1 << 0);

        return parsedStruct;
    }

}