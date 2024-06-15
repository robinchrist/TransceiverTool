#pragma once
#include <string>
#include <array>

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
    };
}