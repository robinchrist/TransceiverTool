#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"

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
    };
}