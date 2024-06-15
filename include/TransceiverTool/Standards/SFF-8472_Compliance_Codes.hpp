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

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    //SONET Compliance Codes (byte 4+5)
    struct SONET_Compliance_Codes {
        bool OC_192_short_reach_byte_4_bit_5;
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
    }
}