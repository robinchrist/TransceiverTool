#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {
    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //10G Ethernet Compliance Codes (byte 3)
    struct _10G_Ethernet_Compliance_Codes {
        bool _10GBASE_ER_bit_7;
        bool _10GBASE_LRM_bit_6;
        bool _10GBASE_LR_bit_5;
        bool _10GBASE_SR_bit_4;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Infiniband Compliance Codes (byte 3)
    struct Infiniband_Compliance_Codes {
        bool _1X_SX_bit_3;
        bool _1X_LX_bit_2;
        bool _1X_Copper_Active_bit_1;
        bool _1X_Copper_Passive_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //ESCON Compliance Codes (byte 4)
    struct ESCON_Compliance_Codes {
        bool ESCON_MMF_1310nm_LED_bit_7;
        bool ESCON_SMF_1310nm_Laser_bit_6;
    };

    enum class SONETReachSpecifier {
        SONET_SR,
        SONET_SR_IR_LR_1,
        SONET_IR_LR_2,
        SONET_LR_3,
    };

    struct SONETReachSpecifierAssignedValue {
        SONETReachSpecifier enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    inline const std::array<SONETReachSpecifierAssignedValue, 8> SONETReachSpecifierAssignedValues {{
        {SONETReachSpecifier::SONET_SR, "SONET SR compliant", 0b00},
        {SONETReachSpecifier::SONET_SR_IR_LR_1, "SONET [SR/IR/LR]-1 compliant", 0b10},
        {SONETReachSpecifier::SONET_IR_LR_2, "SONET [IR/LR]-2 compliant", 0b01},
        {SONETReachSpecifier::SONET_LR_3, "SONET LR-3 compliant", 0b11}
        //0x08 - 0xFF: Reserved
    }};
    const SONETReachSpecifierAssignedValue& getSFF8472_SONETReachSpecifierInfo(SONETReachSpecifier enum_value);
    SONETReachSpecifier getSFF8472_SONETReachSpecifier_From_Char(unsigned char value);

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //SONET Compliance Codes (byte 4+5)
    struct SONET_Compliance_Codes {
        bool OC_192_short_reach_byte_4_bit_5;
        SONETReachSpecifier SONET_Reach_Specifier_byte_4_bit_4_3;
        bool OC_48_long_reach_byte_4_bit_2;
        bool OC_48_intermediate_reach_byte_4_bit_1;
        bool OC_48_short_reach_byte_4_bit_0;
        bool reserved_byte_5_bit_7;
        bool OC_12_single_mode_long_reach_2_byte_5_bit_6;
        bool OC_12_single_mode_intermediate_reach_byte_5_bit_5;
        bool OC_12_short_reach_byte_5_bit_4;
        bool reserved_byte_5_bit_3;
        bool OC_3_single_mode_long_reach_byte_5_bit_2;
        bool OC_3_single_mode_intermediate_reach_byte_5_bit_1;
        bool OC_3_short_reach__byte_5_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Ethernet Compliance Codes (byte 6)
    struct Ethernet_Compliance_Codes {
        bool BASE_PX_bit_7;
        bool BASE_BX10_bit_6;
        bool _100BASE_FX_bit_5;
        bool _100BASE_LX_LX10_bit_4;
        bool _1000BASE_T_bit_3;
        bool _1000BASE_CX_bit_2;
        bool _1000BASE_LX_bit_1;
        bool _1000BASE_SX_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Fibre Channel Link Length (byte 7)
    struct Fibre_Channel_Link_Length_Codes {
        bool very_long_distance_v_bit_7;
        bool short_distance_s_bit_6;
        bool intermediate_distance_i_bit_5;
        bool long_distance_l_bit_4;
        bool medium_distance_m_bit_3;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Fibre Channel Technology (byte 7 + 8)
    struct Fibre_Channel_Technology_Codes {
        bool Shortwave_laser_linear_Rx_SA_byte_7_bit_2;
        bool Longwave_laser_LC_byte_7_bit_1;
        bool Electrical_inter_enclosure_EL_byte_7_bit_0;
        bool Electrical_intra_enclosure_EL_byte_byte_8_bit_7;
        bool Shortwave_laser_w_o_OFC_SN_byte_8_bit_6;
        bool Shortwave_laser_with_OFC_SL_byte_8_bit_5;
        bool Longwave_Laser_LL_byte_8_bit_4;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //SFP+ Cable Technology (byte 8)
    struct SFP_plus_Cable_Technology_Codes {
        bool Active_Cable_bit_3;
        bool Passive_Cable_bit_2;
        bool reserved_bit_1;
        bool reserved_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Fibre Channel Transmission Media (byte 9)
    struct Fibre_Channel_Transmission_Media_Codes {
        bool Twin_Axial_Pair_TW_bit_7;
        bool Twisted_Pair_TP_bit_6;
        bool Miniature_Coax_MI_bit_5;
        bool Video_Coax_TV_bit_4;
        bool Multimode_62_5_um_M6_bit_3;
        bool Multimode_50_um_M5_M5E_bit_2;
        bool reserved_bit_1;
        bool Single_Mode_SM_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Fibre Channel Speed (byte 10)
    struct Fibre_Channel_Speed_Codes {
        bool _1200_MBytes_sec_bit_7;
        bool _800_MBytes_sec_bit_6;
        bool _1600_MBytes_sec_bit_5;
        bool _400_MBytes_sec_bit_4;
        bool _3200_MBytes_sec_bit_3;
        bool _200_MBytes_sec_bit_2;
        bool see_byte_62_fibre_channel_speed_2_bit_1;
        bool _100_MBytes_sec_bit_0;
    };

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //Fibre Channel Speed 2 (byte 62)
    struct Fibre_Channel_Speed_2_Codes {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool reserved_bit_5;
        bool reserved_bit_4;
        bool reserved_bit_3;
        bool reserved_bit_2;
        bool reserved_bit_1;
        bool _64_GFC_bit_0;
    }
}