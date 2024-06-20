#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    enum class RateIdentifier {
        Unspecified,
        SFF_8079_4_2_1G_Rate_Select_and_AS0_AS1,
        SFF_8431_8_4_2G_Rx_Rate_Select_only,
        SFF_8431_8_4_2G_Tx_Rate_Select_only,
        SFF_8431_8_4_2G_Independent_Rx_and_Tx_Rate_select,
        FC_PI_5_16_8_4G_Rx_Rate_select_only,
        FC_PI_5_16_8_4G_Independent_Rx_Tx_Rate_select,
        FC_PI_6_32_16_8G_Independent_Rx_Tx_Rate_Select,
        _10_8G_Rx_and_Tx_Rate_Select,
        FC_PI_7_64_32_16G_Independent_Rx_Tx_Rate_Select,
        Rate_select_based_on_PMDs_as_defined_by_A0h,
    };

    struct RateIdentifierAssignedValue {
        RateIdentifier enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    inline const std::array<RateIdentifierAssignedValue, 11> RateIdentifierAssignedValues {{
        {RateIdentifier::Unspecified, "Unspecified (00h)", 0x00},
        {RateIdentifier::SFF_8079_4_2_1G_Rate_Select_and_AS0_AS1, "SFF-8079 (4/2/1G Rate_Select & AS0/AS1)", 0x01},
        {RateIdentifier::SFF_8431_8_4_2G_Rx_Rate_Select_only, "SFF-8431 (8/4/2G Rx Rate_Select only)", 0x02},
        //03h Unspecified *
        {RateIdentifier::SFF_8431_8_4_2G_Tx_Rate_Select_only, "SFF-8431 (8/4/2G Tx Rate_Select only)", 0x04},
        //05h Unspecified *
        {RateIdentifier::SFF_8431_8_4_2G_Independent_Rx_and_Tx_Rate_select, "SFF-8431 (8/4/2G Independent Rx & Tx Rate_select)", 0x06},
        //07h Unspecified *
        {RateIdentifier::FC_PI_5_16_8_4G_Rx_Rate_select_only, "FC-PI-5 (16/8/4G Rx Rate_select only) High=16G only, Low=8G/4G", 0x08},
        //09h Unspecified *
        {RateIdentifier::FC_PI_5_16_8_4G_Independent_Rx_Tx_Rate_select, "FC-PI-5 (16/8/4G Independent Rx, Tx Rate_select) High=16G only, Low=8G/4G", 0x0A},
        //0Bh Unspecified *
        {RateIdentifier::FC_PI_6_32_16_8G_Independent_Rx_Tx_Rate_Select, "FC-PI-6 (32/16/8G Independent Rx, Tx Rate_Select) High=32G only, Low = 16G/8G", 0x0C},
        //0Dh Unspecified *
        {RateIdentifier::_10_8G_Rx_and_Tx_Rate_Select, "10/8G Rx and Tx Rate_Select controlling the operation or locking modes of the internal signal conditioner, retimer or CDR", 0x0E},
        //0Fh Unspecified *
        {RateIdentifier::FC_PI_7_64_32_16G_Independent_Rx_Tx_Rate_Select, "FC-PI-7 (64/32/16G Independent Rx, Tx Rate Select) High = 32GFC and 64GFC, Low = 16GFC", 0x10},
        //11h Unspecified *
        //12h-1Fh Reserved
        {RateIdentifier::Rate_select_based_on_PMDs_as_defined_by_A0h, "Rate select based on PMDs as defined by A0h, byte 36 and A2h, byte 67 (Rx, Tx Rate Select)", 0x20},
        //21h-FFh Reserved
    }};
    const RateIdentifierAssignedValue& getSFF8472_RateIdentifierInfo(RateIdentifier enum_value);
    RateIdentifier getSFF8472_RateIdentifier_From_Char(unsigned char value);

    std::string byteToRateIdentifierString(unsigned char value);

}