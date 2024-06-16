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

        return parsedStruct;
    }

}