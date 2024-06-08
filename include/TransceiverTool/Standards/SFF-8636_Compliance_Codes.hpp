#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {
    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 131
    //10/40G/100G Ethernet Compliance Codes
    struct _10_40G_100G_Ethernet_Compliance_Codes {
        bool extended_bit_7;
        bool _10GBASE_LRM_bit_6;
        bool _10GBASE_LR_bit_5;
        bool _10GBASE_SR_bit_4;
        bool _40GBASE_CR4_bit_3;
        bool _40GBASE_SR4_bit_2;
        bool _40GBASE_LR4_bit_1;
        bool _40G_Active_Cable_XLPPI_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 132
    //SONET Compliance Codes
    struct SONET_Compliance_Codes {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool reserved_bit_5;
        bool reserved_bit_4;
        bool reserved_bit_3;
        bool OC48_long_reach_bit_2;
        bool OC48_intermediate_reach_bit_1;
        bool OC48_short_reach_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 133
    //SAS/SATA Compliance Codes
    struct SAS_SATA_Compliance_Codes {
        bool SAS_24_0_Gbps_bit_7;
        bool SAS_12_0_Gbps_bit_6;
        bool SAS_6_0_Gbps_bit_5;
        bool SAS_3_0_Gbps_bit_4;
        bool reserved_bit_3;
        bool reserved_bit_2;
        bool reserved_bit_1;
        bool reserved_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 134
    //Gigabit Ethernet Compliance Codes
    struct Gigabit_Ethernet_Compliance_Codes {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool reserved_bit_5;
        bool reserved_bit_4;
        bool _1000BASE_T_bit_3;
        bool _1000BASE_CX_bit_2;
        bool _1000BASE_LX_bit_1;
        bool _1000BASE_SX_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 135
    //Fibre Channel Link Length & Fibre Channel Transmitter Technology
    struct Fibre_Channel_Link_Length_And_Transmitter_Technology {
        bool Very_long_distance_V_bit_7;
        bool Short_distance_S_bit_6;
        bool Intermediate_distance_I_bit_5;
        bool Long_distance_L_bit_4;
        bool Medium_M_bit_3;
        bool reserved_Transmitter_Technology_bit_2;
        bool Longwave_laser_LC_bit_1;
        bool Electrical_inter_enclosure_EL_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 136
    //Fibre Channel Transmitter Technology
    struct Fibre_Channel_Transmitter_Technology {
        bool Electrical_intra_enclosure_bit_7;
        bool Shortwave_laser_w_o_OFC_SN_bit_6;
        bool Shortwave_laser_w_OFC_SL_bit_5;
        bool Longwave_Laser_LL_bit_4;
        bool reserved_bit_3;
        bool reserved_bit_2;
        bool reserved_bit_1;
        bool reserved_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 137
    //Fibre Channel Transmission Media
    struct Fibre_Channel_Transmission_Media {
        bool Twin_Axial_Pair_TW_bit_7;
        bool Shielded_Twisted_Pair_TP_bit_6;
        bool Miniature_Coax_MI_bit_5;
        bool Video_Coax_TV_bit_4;
        bool Multi_mode_62_5_um_M6_bit_3;
        bool Multi_mode_50_um_M5_bit_2;
        bool Multi_mode_50_um_OM3_bit_1;
        bool Single_Mode_SM_bit_0;
    };

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 137
    //Fibre Channel Speed
    struct Fibre_Channel_Speed {
        bool _1200_MBps_per_channel_bit_7;
        bool _800_MBps_bit_6;
        bool _1600_MBps_per_channel_bit_5;
        bool _400_MBps_bit_4;
        bool _3200_MBps_per_channel_bit_3;
        bool _200_MBps_bit_2;
        bool Extended_bit_1;
        bool _100_MBps_bit_0;
    };

}