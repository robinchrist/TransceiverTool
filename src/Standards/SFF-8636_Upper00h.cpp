#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Identifier_Values.hpp"
#include <fmt/core.h>
#include <limits>
#include <stdexcept>
#include <cstdlib>

namespace TransceiverTool::Standards::SFF8636 {

//############
    nlohmann::json charToJSONByteStruct(unsigned char value) {
        nlohmann::json j;

        j["byteValue"] = fmt::format("{:#04x}", value);

        return j;
    }

    unsigned char charFromJSONByteStruct(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Tried to create byte from something that is not an object!");
        
        const auto& byteValue = j.at("byteValue");
        if(!byteValue.is_string()) throw std::invalid_argument("byteValue entry is not a string");

        auto byteValueStr = byteValue.template get<std::string>();
        if(byteValueStr.length() != 4) throw std::invalid_argument("byteValue has wrong length (must be 4)");

        if(byteValueStr[0] != '0' || byteValueStr[1] != 'x') throw std::invalid_argument("byteValue does not start with 0x");
        
        char* outPtr;
        unsigned long longValue = strtoul(byteValueStr.c_str(), &outPtr, 16);
        if(outPtr != byteValueStr.c_str() + 4) throw std::invalid_argument("byteValue is not valid hex number!");
        if(longValue > std::numeric_limits<unsigned char>::max()) throw std::invalid_argument("byteValue exceeds byte range?!");

        return longValue;
    }
//############

//############
    nlohmann::json TransceiverReferenceToJSON(unsigned char byte_value) {
        nlohmann::json j;

        auto it = std::find_if(
            SFF8024::TransceiverReferenceAssignedValues.begin(),
            SFF8024::TransceiverReferenceAssignedValues.end(),
            [byte_value](const SFF8024::TransceiverReferenceAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8024::TransceiverReferenceAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char TransceiverReferenceFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8024::TransceiverReferenceAssignedValues.begin(),
                SFF8024::TransceiverReferenceAssignedValues.end(),
                [&strValue](const SFF8024::TransceiverReferenceAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8024::TransceiverReferenceAssignedValues.end()) throw std::invalid_argument("Identifier is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Identifier has wrong type (neither string nor object)");
        }
    }
//############

//############
    nlohmann::json Extended_IdentifierToJSON(const Extended_Identifier& extIdentifier) {
        nlohmann::json j;

        j["Power Class (Bit 7-6)"] = getSFF8636_Extended_Identifier_Bit_7_6Info(extIdentifier.power_class_bit_7_6).description;
        j["Power Class 8 implemented (Bit 5)"] = extIdentifier.power_class_8_bit_5;
        j["CLEI Code Present (Bit 4)"] = extIdentifier.clei_code_bit_4;
        j["CDR present in Tx (Bit 3)"] = extIdentifier.cdr_in_tx_bit_3;
        j["CDR present in Rx (Bit 2)"] = extIdentifier.cdr_in_rx_bit_2;
        j["Extended Power Class (Bit 1-0)"] = getSFF8636_Extended_Identifier_Bit_1_0Info(extIdentifier.power_class_bit_1_0).description;

        return j;
    }

    Extended_Identifier Extended_IdentifierFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Extended Identifier must be an object");

        Extended_Identifier extIdentifier;

        auto power_class_bit_7_6_str = j.at("Power Class (Bit 7-6)").template get<std::string>();
        auto power_class_bit_7_6It = std::find_if(
            SFF8636::Extended_Identifier_Bit_7_6_strings.begin(),
            SFF8636::Extended_Identifier_Bit_7_6_strings.end(),
            [&power_class_bit_7_6_str](const Extended_Identifier_Bit_7_6_string& value) { return value.description == power_class_bit_7_6_str; }
        );
        if(power_class_bit_7_6It == Extended_Identifier_Bit_7_6_strings.end()) {
            throw std::invalid_argument("Invalid value for Power Class (Bit 7-6) string");
        }
        extIdentifier.power_class_bit_7_6 = power_class_bit_7_6It->enum_value;

        extIdentifier.power_class_8_bit_5 = j.at("Power Class 8 implemented (Bit 5)").template get<bool>();
        extIdentifier.clei_code_bit_4 = j.at("CLEI Code Present (Bit 4)").template get<bool>();
        extIdentifier.cdr_in_tx_bit_3 = j.at("CDR present in Tx (Bit 3)").template get<bool>();
        extIdentifier.cdr_in_rx_bit_2 = j.at("CDR present in Rx (Bit 2)").template get<bool>();
        
        auto power_class_bit_1_0_str = j.at("Extended Power Class (Bit 1-0)").template get<std::string>();
        auto power_class_bit_1_0It = std::find_if(
            Extended_Identifier_Bit_1_0_strings.begin(),
            Extended_Identifier_Bit_1_0_strings.end(),
            [&power_class_bit_1_0_str](const Extended_Identifier_Bit_1_0_string& value) { return value.description == power_class_bit_1_0_str; }
        );
        if(power_class_bit_1_0It == Extended_Identifier_Bit_1_0_strings.end()) {
            throw std::invalid_argument("Invalid value for Extended Power Class (Bit 1-0) string");
        }
        extIdentifier.power_class_bit_1_0 = power_class_bit_1_0It->enum_value;


        return extIdentifier;
    }
//############

//############
    nlohmann::json TransceiverConnectorTypeToJSON(unsigned char byte_value) {
        nlohmann::json j;

        auto it = std::find_if(
            SFF8024::TransceiverConnectorTypeAssignedValues.begin(),
            SFF8024::TransceiverConnectorTypeAssignedValues.end(),
            [byte_value](const SFF8024::TransceiverConnectorTypeAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8024::TransceiverConnectorTypeAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char TransceiverConnectorTypeFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8024::TransceiverConnectorTypeAssignedValues.begin(),
                SFF8024::TransceiverConnectorTypeAssignedValues.end(),
                [&strValue](const SFF8024::TransceiverConnectorTypeAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8024::TransceiverConnectorTypeAssignedValues.end()) throw std::invalid_argument("Connector Type is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Connector Type has wrong type (neither string nor object)");
        }
    }

//############


//############
    nlohmann::json _10_40G_100G_Ethernet_Compliance_CodesToJSON(const _10_40G_100G_Ethernet_Compliance_Codes& value) {
        nlohmann::json j;

        j["Extended (Bit 7)"] = value.extended_bit_7;
        j["10GBASE-LRM compliant (Bit 6)"] = value._10GBASE_LRM_bit_6;
        j["10GBASE-LR compliant (Bit 5)"] = value._10GBASE_LR_bit_5;
        j["10GBASE-SR compliant (Bit 4)"] = value._10GBASE_SR_bit_4;
        j["40GBASE-CR4 compliant (Bit 3)"] = value._40GBASE_CR4_bit_3;
        j["40GBASE-SR4 compliant (Bit 2)"] = value._40GBASE_SR4_bit_2;
        j["40GBASE-LR4 compliant (Bit 1)"] = value._40GBASE_LR4_bit_1;
        j["40G Active Cable (XLPPI) compliant (Bit 0)"] = value._40G_Active_Cable_XLPPI_bit_0;

        return j;
    }

    _10_40G_100G_Ethernet_Compliance_Codes _10_40G_100G_Ethernet_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("10/40G/100G Ethernet Compliance Codes must be an object");

        _10_40G_100G_Ethernet_Compliance_Codes complianceCodes;

        complianceCodes.extended_bit_7 = j.at("Extended (Bit 7)").template get<bool>();
        complianceCodes._10GBASE_LRM_bit_6 = j.at("10GBASE-LRM compliant (Bit 6)").template get<bool>();
        complianceCodes._10GBASE_LR_bit_5 = j.at("10GBASE-LR compliant (Bit 5)").template get<bool>();
        complianceCodes._10GBASE_SR_bit_4 = j.at("10GBASE-SR compliant (Bit 4)").template get<bool>();
        complianceCodes._40GBASE_CR4_bit_3 = j.at("40GBASE-CR4 compliant (Bit 3)").template get<bool>();
        complianceCodes._40GBASE_SR4_bit_2 = j.at("40GBASE-SR4 compliant (Bit 2)").template get<bool>();
        complianceCodes._40GBASE_LR4_bit_1 = j.at("40GBASE-LR4 compliant (Bit 1)").template get<bool>();
        complianceCodes._40G_Active_Cable_XLPPI_bit_0 = j.at("40G Active Cable (XLPPI) compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############

//############
    nlohmann::json SONET_Compliance_CodesToJSON(const SONET_Compliance_Codes& value) {
        nlohmann::json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Reserved (Bit 6)"] = value.reserved_bit_6;
        j["Reserved (Bit 5)"] = value.reserved_bit_5;
        j["Reserved (Bit 4)"] = value.reserved_bit_4;
        j["Reserved (Bit 3)"] = value.reserved_bit_3;
        j["OC 48, long reach compliant (Bit 2)"] = value.OC48_long_reach_bit_2;
        j["OC 48, intermediate reach compliant (Bit 1)"] = value.OC48_intermediate_reach_bit_1;
        j["OC 48, short reach compliant (Bit 0)"] = value.OC48_short_reach_bit_0;

        return j;
    }

    SONET_Compliance_Codes SONET_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("SONET Compliance Codes Compliance Codes must be an object");

        SONET_Compliance_Codes complianceCodes;

        complianceCodes.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        complianceCodes.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        complianceCodes.reserved_bit_5 = j.at("Reserved (Bit 5)").template get<bool>();
        complianceCodes.reserved_bit_4 = j.at("Reserved (Bit 4)").template get<bool>();
        complianceCodes.reserved_bit_3 = j.at("Reserved (Bit 3)").template get<bool>();
        complianceCodes.OC48_long_reach_bit_2 = j.at("OC 48, long reach compliant (Bit 2)").template get<bool>();
        complianceCodes.OC48_intermediate_reach_bit_1 = j.at("OC 48, intermediate reach compliant (Bit 1)").template get<bool>();
        complianceCodes.OC48_short_reach_bit_0 = j.at("OC 48, short reach compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############

//############
    nlohmann::json SAS_SATA_Compliance_CodesToJSON(const SAS_SATA_Compliance_Codes& value) {
        nlohmann::json j;

        j["SAS 24.0 Gbps compliant (Bit 7)"] = value.SAS_24_0_Gbps_bit_7;
        j["SAS 12.0 Gbps compliant (Bit 6)"] = value.SAS_12_0_Gbps_bit_6;
        j["SAS 6.0 Gbps compliant (Bit 5)"] = value.SAS_6_0_Gbps_bit_5;
        j["SAS 3.0 Gbps compliant (Bit 4)"] = value.SAS_3_0_Gbps_bit_4;
        j["Reserved (Bit 3)"] = value.reserved_bit_3;
        j["Reserved (Bit 2)"] = value.reserved_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["Reserved (Bit 0)"] = value.reserved_bit_0;

        return j;
    }

    SAS_SATA_Compliance_Codes SAS_SATA_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("SAS/SATA Compliance Codes must be an object");

        SAS_SATA_Compliance_Codes complianceCodes;

        complianceCodes.SAS_24_0_Gbps_bit_7 = j.at("SAS 24.0 Gbps compliant (Bit 7)").template get<bool>();
        complianceCodes.SAS_12_0_Gbps_bit_6 = j.at("SAS 12.0 Gbps compliant (Bit 6)").template get<bool>();
        complianceCodes.SAS_6_0_Gbps_bit_5 = j.at("SAS 6.0 Gbps compliant (Bit 5)").template get<bool>();
        complianceCodes.SAS_3_0_Gbps_bit_4 = j.at("SAS 3.0 Gbps compliant (Bit 4)").template get<bool>();
        complianceCodes.reserved_bit_3 = j.at("Reserved (Bit 3)").template get<bool>();
        complianceCodes.reserved_bit_2 = j.at("Reserved (Bit 2)").template get<bool>();
        complianceCodes.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        complianceCodes.reserved_bit_0 = j.at("Reserved (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############

//############
    nlohmann::json Gigabit_Ethernet_Compliance_CodesToJSON(const Gigabit_Ethernet_Compliance_Codes& value) {
        nlohmann::json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Reserved (Bit 6)"] = value.reserved_bit_6;
        j["Reserved (Bit 5)"] = value.reserved_bit_5;
        j["Reserved (Bit 4)"] = value.reserved_bit_4;
        j["1000BASE-T compliant (Bit 3)"] = value._1000BASE_T_bit_3;
        j["1000BASE-CX compliant (Bit 2)"] = value._1000BASE_CX_bit_2;
        j["1000BASE-LX compliant (Bit 1)"] = value._1000BASE_LX_bit_1;
        j["1000BASE-SX compliant (Bit 0)"] = value._1000BASE_SX_bit_0;

        return j;
    }

    Gigabit_Ethernet_Compliance_Codes Gigabit_Ethernet_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Gigabit Ethernet Compliance Codes must be an object");

        Gigabit_Ethernet_Compliance_Codes complianceCodes;

        complianceCodes.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        complianceCodes.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        complianceCodes.reserved_bit_5 = j.at("Reserved (Bit 5)").template get<bool>();
        complianceCodes.reserved_bit_4 = j.at("Reserved (Bit 4)").template get<bool>();
        complianceCodes._1000BASE_T_bit_3 = j.at("1000BASE-T compliant (Bit 3)").template get<bool>();
        complianceCodes._1000BASE_CX_bit_2 = j.at("1000BASE-CX compliant (Bit 2)").template get<bool>();
        complianceCodes._1000BASE_LX_bit_1 = j.at("1000BASE-LX compliant (Bit 1)").template get<bool>();
        complianceCodes._1000BASE_SX_bit_0 = j.at("1000BASE-SX compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############

//############
    std::pair<nlohmann::json, nlohmann::json> Fibre_Channel_Link_Length_And_Transmitter_TechnologyToJSON(
        const Fibre_Channel_Link_Length_And_Transmitter_Technology& linkAndTransmitterValue,
        const Fibre_Channel_Transmitter_Technology& transmitterValue
    ) {
        nlohmann::json jlinkValue, jtransmitterValue;

        jlinkValue["Very long distance (V) compliant (Byte 135, Bit 7)"] = linkAndTransmitterValue.Very_long_distance_V_bit_7;
        jlinkValue["Short distance (S) compliant (Byte 135, Bit 6)"] = linkAndTransmitterValue.Short_distance_S_bit_6;
        jlinkValue["Intermediate distance (I) compliant (Byte 135, Bit 5)"] = linkAndTransmitterValue.Intermediate_distance_I_bit_5;
        jlinkValue["Long distance (L) compliant (Byte 135, Bit 4)"] = linkAndTransmitterValue.Long_distance_L_bit_4;
        jlinkValue["Medium distance (M) compliant (Byte 135, Bit 3)"] = linkAndTransmitterValue.Medium_M_bit_3;
    
        jtransmitterValue["Reserved (Byte 135, Bit 2)"] = linkAndTransmitterValue.reserved_Transmitter_Technology_bit_2;
        jtransmitterValue["Longwave laser (LC) compliant (Byte 135, Bit 1)"] = linkAndTransmitterValue.Longwave_laser_LC_bit_1;
        jtransmitterValue["Electrical inter-enclosure (EL) compliant (Byte 135, Bit 0)"] = linkAndTransmitterValue.Electrical_inter_enclosure_EL_bit_0;
        jtransmitterValue["Electrical intra-enclosure compliant (Byte 136, Bit 7)"] = transmitterValue.Electrical_intra_enclosure_bit_7;
        jtransmitterValue["Shortwave laser w/o OFC (SN) compliant (Byte 136, Bit 6)"] = transmitterValue.Shortwave_laser_w_o_OFC_SN_bit_6;
        jtransmitterValue["Shortwave laser w OFC (SL) compliant (Byte 136, Bit 5)"] = transmitterValue.Shortwave_laser_w_OFC_SL_bit_5;
        jtransmitterValue["Longwave Laser (LL) compliant (Byte 136, Bit 4)"] = transmitterValue.Longwave_Laser_LL_bit_4;
        jtransmitterValue["Reserved (Byte 136, Bit 3)"] = transmitterValue.reserved_bit_3;
        jtransmitterValue["Reserved (Byte 136, Bit 2)"] = transmitterValue.reserved_bit_2;
        jtransmitterValue["Reserved (Byte 136, Bit 1)"] = transmitterValue.reserved_bit_1;
        jtransmitterValue["Reserved (Byte 136, Bit 0)"] = transmitterValue.reserved_bit_0;
        

        return std::make_pair(std::move(jlinkValue), std::move(jtransmitterValue));
    }

    std::pair<Fibre_Channel_Link_Length_And_Transmitter_Technology, Fibre_Channel_Transmitter_Technology> Fibre_Channel_Link_Length_And_Transmitter_TechnologyFromJSON(
        const nlohmann::json& jlinkValue,
        const nlohmann::json& jtransmitterValue
    ) {
        if(!jlinkValue.is_object()) throw std::invalid_argument("Fibre Channel Link Length must be an object");
        if(!jtransmitterValue.is_object()) throw std::invalid_argument("Fibre Channel Transmitter Technology must be an object");

        Fibre_Channel_Link_Length_And_Transmitter_Technology linkAndTransmitterValue;
        Fibre_Channel_Transmitter_Technology transmitterValue;

        linkAndTransmitterValue.Very_long_distance_V_bit_7 = jlinkValue.at("Very long distance (V) compliant (Byte 135, Bit 7)").template get<bool>();
        linkAndTransmitterValue.Short_distance_S_bit_6 = jlinkValue.at("Short distance (S) compliant (Byte 135, Bit 6)").template get<bool>();
        linkAndTransmitterValue.Intermediate_distance_I_bit_5 = jlinkValue.at("Intermediate distance (I) compliant (Byte 135, Bit 5)").template get<bool>();
        linkAndTransmitterValue.Long_distance_L_bit_4 = jlinkValue.at("Long distance (L) compliant (Byte 135, Bit 4)").template get<bool>();
        linkAndTransmitterValue.Medium_M_bit_3 = jlinkValue.at("Medium distance (M) compliant (Byte 135, Bit 3)").template get<bool>();
        linkAndTransmitterValue.reserved_Transmitter_Technology_bit_2 = jtransmitterValue.at("Reserved (Byte 135, Bit 2)").template get<bool>();
        linkAndTransmitterValue.Longwave_laser_LC_bit_1 = jtransmitterValue.at("Longwave laser (LC) compliant (Byte 135, Bit 1)").template get<bool>();
        linkAndTransmitterValue.Electrical_inter_enclosure_EL_bit_0 = jtransmitterValue.at("Electrical inter-enclosure (EL) compliant (Byte 135, Bit 0)").template get<bool>();
    
        transmitterValue.Electrical_intra_enclosure_bit_7 = jtransmitterValue.at("Electrical intra-enclosure compliant (Byte 136, Bit 7)").template get<bool>();
        transmitterValue.Shortwave_laser_w_o_OFC_SN_bit_6 = jtransmitterValue.at("Shortwave laser w/o OFC (SN) compliant (Byte 136, Bit 6)").template get<bool>();
        transmitterValue.Shortwave_laser_w_OFC_SL_bit_5 = jtransmitterValue.at("Shortwave laser w OFC (SL) compliant (Byte 136, Bit 5)").template get<bool>();
        transmitterValue.Longwave_Laser_LL_bit_4 = jtransmitterValue.at("Longwave Laser (LL) compliant (Byte 136, Bit 4)").template get<bool>();
        transmitterValue.reserved_bit_3 = jtransmitterValue.at("Reserved (Byte 136, Bit 3)").template get<bool>();
        transmitterValue.reserved_bit_2 = jtransmitterValue.at("Reserved (Byte 136, Bit 2)").template get<bool>();
        transmitterValue.reserved_bit_1 = jtransmitterValue.at("Reserved (Byte 136, Bit 1)").template get<bool>();
        transmitterValue.reserved_bit_0 = jtransmitterValue.at("Reserved (Byte 136, Bit 0)").template get<bool>();

        return std::make_pair(std::move(linkAndTransmitterValue), std::move(transmitterValue));
    }

//############


//############
    nlohmann::json Fibre_Channel_Transmission_MediaToJSON(const Fibre_Channel_Transmission_Media& value) {
        nlohmann::json j;

        j["Twin Axial Pair (TW) compliant (Bit 7)"] = value.Twin_Axial_Pair_TW_bit_7;
        j["Shielded Twisted Pair (TP) compliant (Bit 6)"] = value.Shielded_Twisted_Pair_TP_bit_6;
        j["Miniature Coax (MI) compliant (Bit 5)"] = value.Miniature_Coax_MI_bit_5;
        j["Video Coax (TV) compliant (Bit 4)"] = value.Video_Coax_TV_bit_4;
        j["Multi-mode 62.5 um (M6) compliant (Bit 3)"] = value.Multi_mode_62_5_um_M6_bit_3;
        j["Multi-mode 50 um (M5) compliant (Bit 1)"] = value.Multi_mode_50_um_M5_bit_2;
        j["Multi-mode 50 um (OM3) compliant (Bit 1)"] = value.Multi_mode_50_um_OM3_bit_1;
        j["Single Mode (SM) compliant (Bit 0)"] = value.Single_Mode_SM_bit_0;

        return j;
    }

    Fibre_Channel_Transmission_Media Fibre_Channel_Transmission_MediaFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Transmission Media must be an object");

        Fibre_Channel_Transmission_Media complianceCodes;

        complianceCodes.Twin_Axial_Pair_TW_bit_7 = j.at("Twin Axial Pair (TW) compliant (Bit 7)").template get<bool>();
        complianceCodes.Shielded_Twisted_Pair_TP_bit_6 = j.at("Shielded Twisted Pair (TP) compliant (Bit 6)").template get<bool>();
        complianceCodes.Miniature_Coax_MI_bit_5 = j.at("Miniature Coax (MI) compliant (Bit 5)").template get<bool>();
        complianceCodes.Video_Coax_TV_bit_4 = j.at("Video Coax (TV) compliant (Bit 4)").template get<bool>();
        complianceCodes.Multi_mode_62_5_um_M6_bit_3 = j.at("Multi-mode 62.5 um (M6) compliant (Bit 3)").template get<bool>();
        complianceCodes.Multi_mode_50_um_M5_bit_2 = j.at("Multi-mode 50 um (M5) compliant (Bit 1)").template get<bool>();
        complianceCodes.Multi_mode_50_um_OM3_bit_1 = j.at("Multi-mode 50 um (OM3) compliant (Bit 1)").template get<bool>();
        complianceCodes.Single_Mode_SM_bit_0 = j.at("Single Mode (SM) compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############


//############
    nlohmann::json Fibre_Channel_SpeedToJSON(const Fibre_Channel_Speed& value) {
        nlohmann::json j;

        j["1200 MBps (per channel) compliant (Bit 7)"] = value._1200_MBps_per_channel_bit_7;
        j["800 MBps compliant (Bit 6)"] = value._800_MBps_bit_6;
        j["1600 MBps (per channel) compliant (Bit 5)"] = value._1600_MBps_per_channel_bit_5;
        j["400 MBps compliant (Bit 4)"] = value._400_MBps_bit_4;
        j["3200 MBps (per channel) compliant (Bit 3)"] = value._3200_MBps_per_channel_bit_3;
        j["200 MBps compliant (Bit 2)"] = value._200_MBps_bit_2;
        j["Extended (Bit 1)"] = value.Extended_bit_1;
        j["100 MBps compliant (Bit 0)"] = value._100_MBps_bit_0;

        return j;
    }

    Fibre_Channel_Speed Fibre_Channel_SpeedFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Speed must be an object");

        Fibre_Channel_Speed complianceCodes;

        complianceCodes._1200_MBps_per_channel_bit_7 = j.at("1200 MBps (per channel) compliant (Bit 7)").template get<bool>();
        complianceCodes._800_MBps_bit_6 = j.at("800 MBps compliant (Bit 6)").template get<bool>();
        complianceCodes._1600_MBps_per_channel_bit_5 = j.at("1600 MBps (per channel) compliant (Bit 5)").template get<bool>();
        complianceCodes._400_MBps_bit_4 = j.at("400 MBps compliant (Bit 4)").template get<bool>();
        complianceCodes._3200_MBps_per_channel_bit_3 = j.at("3200 MBps (per channel) compliant (Bit 3)").template get<bool>();
        complianceCodes._200_MBps_bit_2 = j.at("200 MBps compliant (Bit 2)").template get<bool>();
        complianceCodes.Extended_bit_1 = j.at("Extended (Bit 1)").template get<bool>();
        complianceCodes._100_MBps_bit_0 = j.at("100 MBps compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }

//############

    void SFF8636_Upper00hToJSON(nlohmann::json& j, const SFF8636_Upper00h& programming) {

        j["Type"] = "SFF-8636 Upper Page 00h";
        
        j["Identifier"] = TransceiverReferenceToJSON(programming.byte_128_Identifier);

        j["Extended Identifier"] = Extended_IdentifierToJSON(programming.byte_129_extended_identifier);

        j["Connector Type"] = TransceiverConnectorTypeToJSON(programming.byte_130_Connector_Type);

        j["10/40G/100G Ethernet Compliance Codes"] = _10_40G_100G_Ethernet_Compliance_CodesToJSON(programming.byte_131_ethernet_compliance_codes);
        j["SONET Compliance Codes"] = SONET_Compliance_CodesToJSON(programming.byte_132_sonet_compliance_codes);
        j["SAS/SATA Compliance Codes"] = SAS_SATA_Compliance_CodesToJSON(programming.byte_133_sas_sata_compliance_codes);
        j["Gigabit Ethernet Compliance Codes"] = Gigabit_Ethernet_Compliance_CodesToJSON(programming.byte_134_gigabit_ethernet_compliance_codes);
        auto fibreChannelPair = Fibre_Channel_Link_Length_And_Transmitter_TechnologyToJSON(
            programming.byte_135_fibre_channel_link_length_and_transmitter_technology, 
            programming.byte_136_fibre_channel_transmitter_technology
        );
        j["Fibre Channel Link Length"] = std::move(fibreChannelPair.first);
        j["Fibre Channel Transmitter Technology"] = std::move(fibreChannelPair.second);
        j["Fibre Channel Transmission Media"] = Fibre_Channel_Transmission_MediaToJSON(programming.byte_137_fibre_channel_transmission_media);
        j["Fibre Channel Speed"] = Fibre_Channel_SpeedToJSON(programming.byte_138_fibre_channel_speed);
    }


    void SFF8636_Upper00hFromJSON(const nlohmann::json& j, SFF8636_Upper00h programming) {

        if(j.at("Type").template get<std::string>() != "SFF-8636 Upper Page 00h") {
            throw std::invalid_argument("JSON specifies wrong type");
        }

        programming.byte_128_Identifier = TransceiverReferenceFromJSON(j.at("Identifier"));

        programming.byte_129_extended_identifier = Extended_IdentifierFromJSON(j.at("Extended Identifier"));

        programming.byte_130_Connector_Type = TransceiverConnectorTypeFromJSON(j.at("Connector Type"));

        programming.byte_131_ethernet_compliance_codes = _10_40G_100G_Ethernet_Compliance_CodesFromJSON(j.at("10/40G/100G Ethernet Compliance Codes"));
        programming.byte_132_sonet_compliance_codes = SONET_Compliance_CodesFromJSON(j.at("SONET Compliance Codes"));
        programming.byte_133_sas_sata_compliance_codes = SAS_SATA_Compliance_CodesFromJSON(j.at("SAS/SATA Compliance Codes"));
        programming.byte_134_gigabit_ethernet_compliance_codes = Gigabit_Ethernet_Compliance_CodesFromJSON(j.at("Gigabit Ethernet Compliance Codes"));
        auto fibreChannelPair = Fibre_Channel_Link_Length_And_Transmitter_TechnologyFromJSON(j.at("Fibre Channel Link Length"), j.at("Fibre Channel Transmitter Technology"));
        programming.byte_135_fibre_channel_link_length_and_transmitter_technology = std::move(fibreChannelPair.first);
        programming.byte_136_fibre_channel_transmitter_technology = std::move(fibreChannelPair.second);
        programming.byte_137_fibre_channel_transmission_media = Fibre_Channel_Transmission_MediaFromJSON(j.at("Fibre Channel Transmission Media"));
        programming.byte_138_fibre_channel_speed = Fibre_Channel_SpeedFromJSON(j.at("Fibre Channel Speed"));
    }


}