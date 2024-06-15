#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8472 {

    //Expects 256 bytes!
    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h parseBytesToStruct(unsigned char const * bytes) {
        SFF8472_LowerA0h parsedStruct;

        parsedStruct.byte_0_Identifier = bytes[0];

        parsedStruct.byte_1_extended_identifier = bytes[1];

        parsedStruct.byte_2_Connector_type = bytes[2];

        return parsedStruct;
    }

}