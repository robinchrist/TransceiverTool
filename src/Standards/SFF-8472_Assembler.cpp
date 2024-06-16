#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
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
    
    }
}