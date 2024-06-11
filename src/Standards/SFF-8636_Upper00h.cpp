#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8636_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8636_Date_Code.hpp"
#include "TransceiverTool/Standards/SFF-8636_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Vendor_OUIs.hpp"
#include <cctype>
#include <fmt/core.h>
#include <limits>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <cstdlib>
#include <cppcodec/base64_rfc4648.hpp>

namespace TransceiverTool::Standards::SFF8636 {

//############
    nlohmann::ordered_json charToJSONByteStruct(unsigned char value) {
        nlohmann::ordered_json j;

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
        
        if(!std::isxdigit(byteValueStr[2]) || !std::isxdigit(byteValueStr[3])) throw std::invalid_argument("byteValue has invalid characters");
        
        char* outPtr;
        unsigned long longValue = strtoul(byteValueStr.c_str(), &outPtr, 16);
        if(outPtr != byteValueStr.c_str() + 4) throw std::invalid_argument("byteValue is not valid hex number??");
        if(longValue > std::numeric_limits<unsigned char>::max()) throw std::invalid_argument("byteValue exceeds byte range?!");

        return longValue;
    }
//############

//############
    nlohmann::ordered_json TransceiverReferenceToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json Extended_IdentifierToJSON(const Extended_Identifier& extIdentifier) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json TransceiverConnectorTypeToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json _10_40G_100G_Ethernet_Compliance_CodesToJSON(const _10_40G_100G_Ethernet_Compliance_Codes& value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json SONET_Compliance_CodesToJSON(const SONET_Compliance_Codes& value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json SAS_SATA_Compliance_CodesToJSON(const SAS_SATA_Compliance_Codes& value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json Gigabit_Ethernet_Compliance_CodesToJSON(const Gigabit_Ethernet_Compliance_Codes& value) {
        nlohmann::ordered_json j;

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
        nlohmann::ordered_json jlinkValue, jtransmitterValue;

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
    nlohmann::ordered_json Fibre_Channel_Transmission_MediaToJSON(const Fibre_Channel_Transmission_Media& value) {
        nlohmann::ordered_json j;

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
    nlohmann::ordered_json Fibre_Channel_SpeedToJSON(const Fibre_Channel_Speed& value) {
        nlohmann::ordered_json j;

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

//############
    nlohmann::ordered_json EncodingToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8024::SFF8636TransceiverEncodingAssignedValues.begin(),
            SFF8024::SFF8636TransceiverEncodingAssignedValues.end(),
            [byte_value](const SFF8024::SFF8636TransceiverEncodingAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8024::SFF8636TransceiverEncodingAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char EncodingFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8024::SFF8636TransceiverEncodingAssignedValues.begin(),
                SFF8024::SFF8636TransceiverEncodingAssignedValues.end(),
                [&strValue](const SFF8024::SFF8636TransceiverEncodingAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8024::SFF8636TransceiverEncodingAssignedValues.end()) throw std::invalid_argument("Encoding is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Encoding has wrong type (neither string nor object)");
        }
    }
//############

//############
    nlohmann::ordered_json NominalSignalingRate100MBaudToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        if(byte_value != 0xFF) {
            j = (unsigned long)(byte_value) * 100ul;
        } else {
            j = "> 25.4 GBd";
        }

        return j;
    }

    unsigned char NominalSignalingRate100MBaudFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            if(strValue != "> 25.4 GBd") throw std::invalid_argument("Nominal Signaling Rate can only have value > 25.4 GBd if string");

            return 0xFF;
        } else if(j.is_number_unsigned()) {
            auto numberValue = j.template get<std::uint64_t>();

            if(numberValue % 100 != 0) throw std::invalid_argument("Nominal Signaling Rate must be divisble by 100!");

            if(numberValue > 25400) throw std::invalid_argument("Nominal Signaling Rate must not be larger than 25400");

            return numberValue / 100;
        } else {
            throw std::invalid_argument("Nominal Signaling Rate has wrong type (neither string nor unsigned integer)");
        }
    }
//############

//############
    nlohmann::ordered_json Extended_Rate_Select_ComplianceToJSON(const Extended_Rate_Select_Compliance& extRateSelectCompliance) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = extRateSelectCompliance.reserved_bit_7;
        j["Reserved (Bit 6)"] = extRateSelectCompliance.reserved_bit_6;
        j["Reserved (Bit 5)"] = extRateSelectCompliance.reserved_bit_5;
        j["Reserved (Bit 4)"] = extRateSelectCompliance.reserved_bit_4;
        j["Reserved (Bit 3)"] = extRateSelectCompliance.reserved_bit_3;
        j["Reserved (Bit 2)"] = extRateSelectCompliance.reserved_bit_2;
        j["Rate Select Version (Bit 1-0)"] = getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(extRateSelectCompliance.rate_select_bits_1_0).description;

        return j;
    }

    Extended_Rate_Select_Compliance Extended_Rate_Select_ComplianceJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Extended Rate Select Compliance must be an object");

        Extended_Rate_Select_Compliance extRateSelectCompliance;

        extRateSelectCompliance.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        extRateSelectCompliance.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        extRateSelectCompliance.reserved_bit_5 = j.at("Reserved (Bit 5)").template get<bool>();
        extRateSelectCompliance.reserved_bit_4 = j.at("Reserved (Bit 4)").template get<bool>();
        extRateSelectCompliance.reserved_bit_3 = j.at("Reserved (Bit 3)").template get<bool>();
        extRateSelectCompliance.reserved_bit_2 = j.at("Reserved (Bit 2)").template get<bool>();

    
        auto rate_select_bits_1_0_str = j.at("Rate Select Version (Bit 1-0)").template get<std::string>();
        auto rate_select_bits_1_0It = std::find_if(
            Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
            Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
            [&rate_select_bits_1_0_str](const Extended_Rate_Select_Compliance_Bit_1_0_string& value) { return value.description == rate_select_bits_1_0_str; }
        );
        if(rate_select_bits_1_0It == Extended_Rate_Select_Compliance_Bit_1_0_strings.end()) {
            throw std::invalid_argument("Invalid value for Rate Select Version (Bit 1-0) string");
        }
        extRateSelectCompliance.rate_select_bits_1_0 = rate_select_bits_1_0It->enum_value;


        return extRateSelectCompliance;
    }
//############

//############
    nlohmann::ordered_json LengthSMFkmToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        if(byte_value == 0) {
            j = "N/A";
        } else if (j == 1) {
            j = "0 - 1";
        } else {
            j = (unsigned long)(byte_value);
        }

        return j;
    }

    unsigned char LengthSMFkmFromJSON(const nlohmann::json& j) {

        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            if(strValue == "N/A") return 0;
            if(strValue == "0 - 1") return 1;

            throw std::invalid_argument("Length (Standard SM Fiber) [km] can only have value N/A and 0 - 1 if string");
        } else if(j.is_number_unsigned()) {
            auto numberValue = j.template get<std::uint64_t>();

            if(numberValue > 255) throw std::invalid_argument("Length (Standard SM Fiber) [km] must not be larger than 255");

            return numberValue;
        } else {
            throw std::invalid_argument("Length (Standard SM Fiber) [km] has wrong type (neither string nor unsigned integer)");
        }
    }
//############

//############
    nlohmann::ordered_json LengthOM3mToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        if(byte_value == 0) {
            j = "N/A";
        } else {
            j = (unsigned long)(byte_value) * 2ul;
        }

        return j;
    }

    unsigned char LengthOM3mFromJSON(const nlohmann::json& j) {

        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            if(strValue == "N/A") return 0;

            throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 2) can only have value N/A if string");
        } else if(j.is_number_unsigned()) {
            auto numberValue = j.template get<std::uint64_t>();

            if(numberValue % 2 != 0) throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 2) must be divisble by 2");
            if(numberValue > 510) throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 2) must not be larger than 510");

            return numberValue / 2;
        } else {
            throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 2) has wrong type (neither string nor unsigned integer)");
        }
    }
//############

//############
    nlohmann::ordered_json LengthOM2mToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        if(byte_value == 0) {
            j = "N/A";
        } else {
            j = (unsigned long)(byte_value);
        }

        return j;
    }

    unsigned char LengthOM2mFromJSON(const nlohmann::json& j) {

        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            if(strValue == "N/A") return 0;

            throw std::invalid_argument("Length (OM2 50 um) [m] can only have value N/A if string");
        } else if(j.is_number_unsigned()) {
            auto numberValue = j.template get<std::uint64_t>();

            if(numberValue > 255) throw std::invalid_argument("Length (OM2 50 um) [m] must not be larger than 255");

            return numberValue;
        } else {
            throw std::invalid_argument("Length (OM2 50 um) [m] has wrong type (neither string nor unsigned integer)");
        }
    }
//############


//############
    nlohmann::ordered_json CopperOrFibreInfoToJSON(
        bool copperMode,
        unsigned char byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB,
        unsigned char byte_146_length_copper_in_1m_or_om4_in_2m,
        unsigned char byte_186_wavelength_high_order_or_copper_attenuation,
        unsigned char byte_187_wavelength_low_order_or_copper_attenuation,
        unsigned char byte_188_wavelength_tolerance_high_order_or_copper_attenuation,
        unsigned char byte_189_wavelength_tolerance_low_order_or_copper_attenuation
    ) {
        nlohmann::ordered_json j;
        if(copperMode) {
            j["Type"] = "Copper";

            if(byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB == 0) {
                j["Copper Cable Attenuation @ 25.78 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 25.78 GHz [dB]"] = (unsigned long)(byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB);
            }

            if(byte_146_length_copper_in_1m_or_om4_in_2m == 0xFF) {
                j["Length (passive copper or active cable) [m]"] = "> 254 m";
            } else if(byte_146_length_copper_in_1m_or_om4_in_2m == 0) {
                j["Length (passive copper or active cable) [m]"] = "N/A";
            } else {
                j["Length (passive copper or active cable) [m]"] = (unsigned long)(byte_146_length_copper_in_1m_or_om4_in_2m);
            }
            
            if(byte_186_wavelength_high_order_or_copper_attenuation == 0) {
                j["Copper Cable Attenuation @ 2.5 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 2.5 GHz [dB]"] = (unsigned long)(byte_186_wavelength_high_order_or_copper_attenuation);
            }

            if(byte_187_wavelength_low_order_or_copper_attenuation == 0) {
                j["Copper Cable Attenuation @ 5.0 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 5.0 GHz [dB]"] = (unsigned long)(byte_187_wavelength_low_order_or_copper_attenuation);
            }

            if(byte_188_wavelength_tolerance_high_order_or_copper_attenuation == 0) {
                j["Copper Cable Attenuation @ 7.0 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 7.0 GHz [dB]"] = (unsigned long)(byte_188_wavelength_tolerance_high_order_or_copper_attenuation);
            }

            if(byte_188_wavelength_tolerance_high_order_or_copper_attenuation == 0) {
                j["Copper Cable Attenuation @ 12.9 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 12.9 GHz [dB]"] = (unsigned long)(byte_189_wavelength_tolerance_low_order_or_copper_attenuation);
            }
        } else {
            j["Type"] = "Fibre";

            if(byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB == 0) {
                j["Length (OM1 62.5 um) [m]"] = "N/A";
            } else {
                j["Length (OM1 62.5 um) [m]"] = (unsigned long)(byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB);
            }

            if(byte_146_length_copper_in_1m_or_om4_in_2m == 0xFF) {
                j["Length (OM4 50 um) [m] (Divisible by 2)"] = "> 508 m";
            } else if(byte_146_length_copper_in_1m_or_om4_in_2m == 0) {
                j["Length (OM4 50 um) [m] (Divisible by 2)"] = "N/A";
            } else {
                j["Length (OM4 50 um) [m] (Divisible by 2)"] = (unsigned long)(byte_146_length_copper_in_1m_or_om4_in_2m) * 2ul;
            }

            unsigned int wavelengthRaw = 0;
            wavelengthRaw |= (unsigned(byte_186_wavelength_high_order_or_copper_attenuation) << 8);
            wavelengthRaw |= (unsigned(byte_187_wavelength_low_order_or_copper_attenuation) << 0);
            double wavelength = double(wavelengthRaw) / 20.0;
            j["Wavelength [nm] (Divisible by 0.05)"] = wavelength;

            unsigned int wavelengthToleranceRaw = 0;
            wavelengthToleranceRaw |= (unsigned(byte_188_wavelength_tolerance_high_order_or_copper_attenuation) << 8);
            wavelengthToleranceRaw |= (unsigned(byte_189_wavelength_tolerance_low_order_or_copper_attenuation) << 0);
            double wavelengthTolerance = double(wavelengthToleranceRaw) / 200.0;
            j["Wavelength Tolerance [nm] (Divisible by 0.005)"] = wavelengthTolerance;
        }


        return j;
    }

    struct CopperOrFibreInfoFromJSONReturn {
        unsigned char byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB;
        unsigned char byte_146_length_copper_in_1m_or_om4_in_2m;
        unsigned char byte_186_wavelength_high_order_or_copper_attenuation;
        unsigned char byte_187_wavelength_low_order_or_copper_attenuation;
        unsigned char byte_188_wavelength_tolerance_high_order_or_copper_attenuation;
        unsigned char byte_189_wavelength_tolerance_low_order_or_copper_attenuation;
    };

    CopperOrFibreInfoFromJSONReturn CopperOrFibreInfoFromJSON(const nlohmann::json& j) {

        if(!j.is_object()) throw std::invalid_argument("Copper or Fibre Properties must be an object");

        CopperOrFibreInfoFromJSONReturn parsedStruct;

        auto typeStr = j.at("Type").template get<std::string>();

        if(typeStr == "Copper") {
            const auto& copper_cable_attenuation_25ghz_val = j.at("Copper Cable Attenuation @ 25.78 GHz [dB]");
            if(copper_cable_attenuation_25ghz_val.is_string()) {
                if(copper_cable_attenuation_25ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = 0;
            } else if(copper_cable_attenuation_25ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_25ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] must not be greater than 255");

                parsedStruct.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& cable_length_val = j.at("Length (passive copper or active cable) [m]");
            if(cable_length_val.is_string()) {
                if(cable_length_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = 0;
                } else if(cable_length_val.template get<std::string>() == "> 254 m") {
                    parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (passive copper or active cable) [m] has string value, must be N/A or > 254 m");
                }
            } else if(cable_length_val.is_number_unsigned()) {
                auto numberValue = cable_length_val.template get<std::uint64_t>();

                if(numberValue > 254) throw std::invalid_argument("Length (passive copper or active cable) [m] must not be greater than 254");

                parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = numberValue;
            } else {
                throw std::invalid_argument("Length (passive copper or active cable) [m] has wrong type (neither string nor unsigned integer)");
            }

            const auto& copper_cable_attenuation_2_5ghz_val = j.at("Copper Cable Attenuation @ 2.5 GHz [dB]");
            if(copper_cable_attenuation_2_5ghz_val.is_string()) {
                if(copper_cable_attenuation_2_5ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 2.5 GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_186_wavelength_high_order_or_copper_attenuation = 0;
            } else if(copper_cable_attenuation_2_5ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_2_5ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 2.5 GHz [dB] must not be greater than 255");

                parsedStruct.byte_186_wavelength_high_order_or_copper_attenuation = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 2.5 GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& copper_cable_attenuation_5_0ghz_val = j.at("Copper Cable Attenuation @ 5.0 GHz [dB]");
            if(copper_cable_attenuation_5_0ghz_val.is_string()) {
                if(copper_cable_attenuation_5_0ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 5.0 GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_187_wavelength_low_order_or_copper_attenuation = 0;
            } else if(copper_cable_attenuation_5_0ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_5_0ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 5.0 GHz [dB] must not be greater than 255");

                parsedStruct.byte_187_wavelength_low_order_or_copper_attenuation = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 5.0 GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& copper_cable_attenuation_7_0ghz_val = j.at("Copper Cable Attenuation @ 7.0 GHz [dB]");
            if(copper_cable_attenuation_7_0ghz_val.is_string()) {
                if(copper_cable_attenuation_7_0ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 7.0 GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_188_wavelength_tolerance_high_order_or_copper_attenuation = 0;
            } else if(copper_cable_attenuation_7_0ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_7_0ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 7.0 GHz [dB] must not be greater than 255");

                parsedStruct.byte_188_wavelength_tolerance_high_order_or_copper_attenuation = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 7.0 GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& copper_cable_attenuation_12_9ghz_val = j.at("Copper Cable Attenuation @ 12.9 GHz [dB]");
            if(copper_cable_attenuation_12_9ghz_val.is_string()) {
                if(copper_cable_attenuation_12_9ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_189_wavelength_tolerance_low_order_or_copper_attenuation = 0;
            } else if(copper_cable_attenuation_12_9ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_12_9ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] must not be greater than 255");

                parsedStruct.byte_189_wavelength_tolerance_low_order_or_copper_attenuation = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] has wrong type (neither string nor unsigned integer)");
            }
        } else if(typeStr == "Fibre") {

            const auto& length_om1_val = j.at("Length (OM1 62.5 um) [m]");
            if(length_om1_val.is_string()) {
                if(length_om1_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = 0;
                } else {
                    throw std::invalid_argument("Length (OM1 62.5 um) [m] has string value, must be N/A ");
                }
            } else if(length_om1_val.is_number_unsigned()) {
                auto numberValue = length_om1_val.template get<std::uint64_t>();

                if(numberValue > 254) throw std::invalid_argument("Length (OM1 62.5 um) [m] must not be greater than 254");

                parsedStruct.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = numberValue;
            } else {
                throw std::invalid_argument("Length (OM1 62.5 um) [m] has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om4_val = j.at("Length (OM4 50 um) [m] (Divisible by 2)");
            if(length_om4_val.is_string()) {
                if(length_om4_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = 0;
                } else if(length_om4_val.template get<std::string>() == "> 508 m") {
                    parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 2) has string value, must be N/A or > 508 m");
                }
            } else if(length_om4_val.is_number_unsigned()) {
                auto numberValue = length_om4_val.template get<std::uint64_t>();

                if(numberValue % 2 != 0) throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 2) must be divisible by 2");
                if(numberValue > 508) throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 2) must not be greater than 508");

                parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = numberValue / 2;
            } else {
                throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 2) has wrong type (neither string nor unsigned integer)");
            }

            const auto& wavelengthVal = j.at("Wavelength [nm] (Divisible by 0.05)");
            if(!wavelengthVal.is_number_float()) throw std::invalid_argument("Wavelength [nm] (Divisible by 0.05) must be a floating point number");

            double rawWavelength = wavelengthVal.template get<double>();
            if(rawWavelength < 0.0) throw std::invalid_argument("Wavelength [nm] (Divisible by 0.05) must be positive");
            if(rawWavelength > 3276.75) throw std::invalid_argument("Wavelength [nm] (Divisible by 0.05) must not be greater than 3276.75");
            //Allow for some FP rounding error due to round trip
            if(std::abs(rawWavelength / 0.05 - std::round(rawWavelength / 0.05)) > 1e-04 ) throw std::invalid_argument("Wavelength [nm] (Divisible by 0.05) must be divisible by 0.05");

            unsigned int wavelengthUint = std::min(65535.0, std::round(rawWavelength * 20.0));

            parsedStruct.byte_186_wavelength_high_order_or_copper_attenuation = (wavelengthUint >> 8) & (0b11111111);
            parsedStruct.byte_187_wavelength_low_order_or_copper_attenuation = (wavelengthUint >> 0) & (0b11111111);



            const auto& wavelengthToleranceVal = j.at("Wavelength Tolerance [nm] (Divisible by 0.005)");
            if(!wavelengthToleranceVal.is_number_float()) throw std::invalid_argument("Wavelength Tolerance [nm] (Divisible by 0.005) must be a floating point number");

            double rawWavelengthTolerance = wavelengthToleranceVal.template get<double>();
            if(rawWavelengthTolerance < 0.0) throw std::invalid_argument("Wavelength Tolerance [nm] (Divisible by 0.005) must be positive");
            if(rawWavelengthTolerance > 327.675) throw std::invalid_argument("Wavelength Tolerance [nm] (Divisible by 0.005) must not be greater than 327.675");
            //Allow for some FP rounding error due to round trip
            if(std::abs(rawWavelengthTolerance / 0.005 - std::round(rawWavelengthTolerance / 0.005)) > 1e-04 ) throw std::invalid_argument("Wavelength [nm] (Divisible by 0.005) must be divisible by 0.005");

            unsigned int wavelengthToleranceUint = std::min(65535.0, std::round(rawWavelengthTolerance * 200.0));

            parsedStruct.byte_188_wavelength_tolerance_high_order_or_copper_attenuation = (wavelengthToleranceUint >> 8) & (0b11111111);
            parsedStruct.byte_189_wavelength_tolerance_low_order_or_copper_attenuation = (wavelengthToleranceUint >> 0) & (0b11111111);

        } else {
            throw std::invalid_argument("Copper or Fibre Properties has invalid Type string, must be Copper or Fibre");
        }

        return parsedStruct;
    }
//############

//############
    nlohmann::ordered_json Device_Technology_and_Transmitter_TechnologyToJSON(const Device_Technology_and_Transmitter_Technology& value) {
        nlohmann::ordered_json j;

        j["Transmitter Technology (Bit 7-4)"] = getSFF8636_Transmitter_Technology_bit_7_4Info(value.transmitter_Technology_bit_7_4).description;
        j["Active Wavelength Control (Bit 3)"] = value.wavelength_control_bit_3;
        j["Cooled Transmitter (Bit 2)"] = value.cooled_transmitter_bit_2;
        j["Detector Type (Bit 1)"] = value.pin_apd_detector_bit_1 ? "APD" : "PIN";
        j["Tunable Transmitter (Bit 0)"] = value.transmitter_tunable_bit_0;

        return j;
    }

    Device_Technology_and_Transmitter_Technology Device_Technology_and_Transmitter_TechnologyFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Device & Transmitter Properties must be an object");
        
        Device_Technology_and_Transmitter_Technology tech;

        auto transmitterTechnologyStr = j.at("Transmitter Technology (Bit 7-4)").template get<std::string>();
        auto it = std::find_if(
            Transmitter_Technology_bit_7_4_strings.begin(),
            Transmitter_Technology_bit_7_4_strings.end(),
            [&transmitterTechnologyStr](const Transmitter_Technology_bit_7_4_string& entry) { return entry.description == transmitterTechnologyStr; }
        );

        if(it == Transmitter_Technology_bit_7_4_strings.end()) throw std::invalid_argument("Transmitter Technology is not a known string value");

        tech.transmitter_Technology_bit_7_4 = it->enum_value;

        tech.wavelength_control_bit_3 = j.at("Active Wavelength Control (Bit 3)").template get<bool>();
        tech.cooled_transmitter_bit_2 = j.at("Cooled Transmitter (Bit 2)").template get<bool>();

        auto detectorTypeStr = j.at("Detector Type (Bit 1)").template get<std::string>();
        if(detectorTypeStr == "APD") {
            tech.pin_apd_detector_bit_1 = true;
        } else if(detectorTypeStr == "PIN") {
            tech.pin_apd_detector_bit_1 = false;
        } else {
            throw std::invalid_argument("Detector Type (Bit 1) invalid string value, must be APD or PIN");
        }

        tech.transmitter_tunable_bit_0 = j.at("Tunable Transmitter (Bit 0)").template get<bool>();

        return tech;
    }

//############

//############
    nlohmann::ordered_json VendorNameToJSON(const std::array<unsigned char, 16>& vendorNameRaw) {
        bool vendorNamePrintable = std::all_of(
            vendorNameRaw.begin(),
            vendorNameRaw.end(),
            [](char c) {return std::isprint(c); }
        );

        nlohmann::ordered_json j;

    
        if(vendorNamePrintable) {
            std::string vendorName = std::string(reinterpret_cast<char const *>(vendorNameRaw.data()), vendorNameRaw.size());
            //rtrim
            vendorName.erase(std::find_if(vendorName.rbegin(), vendorName.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorName.end());

            j = vendorName;
        } else {
            j["Type"] = "Base64";
            j["Value"] = cppcodec::base64_rfc4648::encode(vendorNameRaw.data(), vendorNameRaw.size());
        }

        return j;
    }

    std::array<unsigned char, 16> VendorNameFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 16> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();
            //We don't care whether it contains unprintable characters - when we serialise and check, it's only to prevent invalid JSON being generated
            //However, as this is a parsed value, we know that the JSON must be valid. Let the user enter what they want...
            if(strVal.size() > 16) throw std::invalid_argument("Vendor Name must not have more than 16 characters");

            if(strVal.size() < 16) strVal.resize(16, 0x20);

            std::memcpy(arr.data(), strVal.data(), strVal.size());
        } else if(j.is_object()) {
            if(j.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Name object must have Type Base64");

            auto encodedVal = j.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 16) throw std::invalid_argument("Vendor Name specified as base64 must have exactly 16 characters!");

            std::memcpy(arr.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Name has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json Extended_Module_CodesToJSON(const Extended_Module_Codes& codes) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = codes.reserved_bit_7;
        j["Reserved (Bit 6)"] = codes.reserved_bit_6;
        j["HDR (200G) Infiniband supported (Bit 5)"] = codes.HDR_bit_5;
        j["EDR (100G) Infiniband supported (Bit 4)"] = codes.EDR_bit_4;
        j["FDR (56G) Infiniband supported (Bit 3)"] = codes.FDR_bit_3;
        j["QDR (40G) Infiniband supported (Bit 2)"] = codes.QDR_bit_2;
        j["DDR (20G) Infiniband supported (Bit 1)"] = codes.DDR_bit_1;
        j["SDR (10G) Infiniband supported (Bit 0)"] = codes.SDR_bit_0;

        return j;
    }

    Extended_Module_Codes Extended_Module_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Extended Module Codes (Infiniband) must be an object");

        Extended_Module_Codes codes;

        codes.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        codes.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        codes.HDR_bit_5 = j.at("HDR (200G) Infiniband supported (Bit 5)").template get<bool>();
        codes.EDR_bit_4 = j.at("EDR (100G) Infiniband supported (Bit 4)").template get<bool>();
        codes.FDR_bit_3 = j.at("FDR (56G) Infiniband supported (Bit 3)").template get<bool>();
        codes.QDR_bit_2 = j.at("QDR (40G) Infiniband supported (Bit 2)").template get<bool>();
        codes.DDR_bit_1 = j.at("DDR (20G) Infiniband supported (Bit 1)").template get<bool>();
        codes.SDR_bit_0 = j.at("SDR (10G) Infiniband supported (Bit 0)").template get<bool>();

        return codes;
    }
//############

//############
    nlohmann::ordered_json VendorOUIToJSON(const std::array<unsigned char, 3>& vendorOUI) {
        nlohmann::ordered_json j;


        auto VendorOUIsIt = std::find_if(
            TransceiverTool::VendorOUIs.begin(),
            TransceiverTool::VendorOUIs.end(),
            [seek = vendorOUI](const VendorOUI elem) { return elem.byte_value == seek; }
        );

        if(VendorOUIsIt != TransceiverTool::VendorOUIs.end()) {
            j["Vendor Name"] = VendorOUIsIt->name;
        } else {
            j = fmt::format("{:02x}:{:02x}:{:02x}", vendorOUI[0], vendorOUI[1], vendorOUI[2]);
        }

        return j;
    }

    std::array<unsigned char, 3> VendorOUIFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 3> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();

            if(strVal.size() != 8) throw std::invalid_argument("Vendor OUI string must be exactly 8 characters long");

            if(!std::isxdigit(strVal[0]) || !std::isxdigit(strVal[1]) || strVal[2] != ':' ||
                !std::isxdigit(strVal[3]) || !std::isxdigit(strVal[4]) || strVal[5] != ':' ||
                !std::isxdigit(strVal[6]) || !std::isxdigit(strVal[7])
            ) {
                throw std::invalid_argument("Vendor OUI string has invalid characters or format");
            }

            unsigned long longValue = strtoul(strVal.c_str(), nullptr, 16);
            if(longValue > std::numeric_limits<unsigned char>::max()) throw std::invalid_argument("byteValue exceeds byte range?!");
            arr[0] = longValue;

            longValue = strtoul(strVal.c_str() + 3, nullptr, 16);
            if(longValue > std::numeric_limits<unsigned char>::max()) throw std::invalid_argument("byteValue exceeds byte range?!");
            arr[1] = longValue;

            longValue = strtoul(strVal.c_str() + 6, nullptr, 16);
            if(longValue > std::numeric_limits<unsigned char>::max()) throw std::invalid_argument("byteValue exceeds byte range?!");
            arr[2] = longValue;
        } else if(j.is_object()) {

            auto vendorName = j.at("Vendor Name").template get<std::string>();

            auto VendorOUIsIt = std::find_if(
                TransceiverTool::VendorOUIs.begin(),
                TransceiverTool::VendorOUIs.end(),
                [&vendorName](const VendorOUI elem) { return elem.name == vendorName; }
            );
            if(VendorOUIsIt == TransceiverTool::VendorOUIs.end()) throw std::invalid_argument("Vendor OUI Vendor Name not known");

            arr = VendorOUIsIt->byte_value;
        } else {
            throw std::invalid_argument("Vendor OUI has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json VendorPNToJSON(const std::array<unsigned char, 16>& vendorPNRaw) {
        bool vendorPNPrintable = std::all_of(
            vendorPNRaw.begin(),
            vendorPNRaw.end(),
            [](char c) {return std::isprint(c); }
        );

        nlohmann::ordered_json j;

    
        if(vendorPNPrintable) {
            std::string vendorPN = std::string(reinterpret_cast<char const *>(vendorPNRaw.data()), vendorPNRaw.size());
            //rtrim
            vendorPN.erase(std::find_if(vendorPN.rbegin(), vendorPN.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorPN.end());

            j = vendorPN;
        } else {
            j["Type"] = "Base64";
            j["Value"] = cppcodec::base64_rfc4648::encode(vendorPNRaw.data(), vendorPNRaw.size());
        }

        return j;
    }

    std::array<unsigned char, 16> VendorPNFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 16> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();
            //We don't care whether it contains unprintable characters - when we serialise and check, it's only to prevent invalid JSON being generated
            //However, as this is a parsed value, we know that the JSON must be valid. Let the user enter what they want...
            if(strVal.size() > 16) throw std::invalid_argument("Vendor Part Number must not have more than 16 characters");

            if(strVal.size() < 16) strVal.resize(16, 0x20);

            std::memcpy(arr.data(), strVal.data(), strVal.size());
        } else if(j.is_object()) {
            if(j.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Part Number object must have Type Base64");

            auto encodedVal = j.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 16) throw std::invalid_argument("Vendor Part Number specified as base64 must have exactly 16 characters!");

            std::memcpy(arr.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Part Number has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json VendorRevToJSON(const std::array<unsigned char, 2>& vendorRevRaw) {
        bool vendorRevPrintable = std::all_of(vendorRevRaw.begin(), vendorRevRaw.end(), [](char c) {return std::isprint(c); });

        nlohmann::ordered_json j;

    
        if(vendorRevPrintable) {
            std::string vendorRev = std::string(reinterpret_cast<char const *>(vendorRevRaw.data()), vendorRevRaw.size());
            //rtrim
            vendorRev.erase(std::find_if(vendorRev.rbegin(), vendorRev.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorRev.end());

            j = vendorRev;
        } else {
            j["Type"] = "Base64";
            j["Value"] = cppcodec::base64_rfc4648::encode(vendorRevRaw.data(), vendorRevRaw.size());
        }

        return j;
    }

    std::array<unsigned char, 2> VendorRevFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 2> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();
            //We don't care whether it contains unprintable characters - when we serialise and check, it's only to prevent invalid JSON being generated
            //However, as this is a parsed value, we know that the JSON must be valid. Let the user enter what they want...
            if(strVal.size() > 2) throw std::invalid_argument("Vendor Rev must not have more than 2 characters");

            if(strVal.size() < 2) strVal.resize(2, 0x20);

            std::memcpy(arr.data(), strVal.data(), strVal.size());
        } else if(j.is_object()) {
            if(j.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Rev object must have Type Base64");

            auto encodedVal = j.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Vendor Rev specified as base64 must have exactly 2 characters!");

            std::memcpy(arr.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Rev has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json MaxCaseTemperatureToJSON(unsigned char maxCaseTemperature) {
        nlohmann::ordered_json j;

        if(maxCaseTemperature == 0) maxCaseTemperature = 70;

        j = (unsigned long)(maxCaseTemperature);

        return j;
    }

    unsigned char MaxCaseTemperatureFromJSON(const nlohmann::json& j) {
        auto numberValue = j.template get<std::uint64_t>();

        if(numberValue > 255) throw std::invalid_argument("Maximum Case Temperature [degC] must not be larger than 255");

        if(numberValue == 70) numberValue = 0;

        return numberValue;
    }
//############


//############
    nlohmann::ordered_json ExtendedComplianceCodesToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8024::ExtendedComplianceCodesAssignedValues.begin(),
            SFF8024::ExtendedComplianceCodesAssignedValues.end(),
            [byte_value](const SFF8024::ExtendedComplianceCodesAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8024::ExtendedComplianceCodesAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char ExtendedComplianceCodesFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8024::ExtendedComplianceCodesAssignedValues.begin(),
                SFF8024::ExtendedComplianceCodesAssignedValues.end(),
                [&strValue](const SFF8024::ExtendedComplianceCodesAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8024::ExtendedComplianceCodesAssignedValues.end()) throw std::invalid_argument("Extended Specification Compliance Codes is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument(" Extended Specification Compliance Codes has wrong type (neither string nor object)");
        }
    }

//############


//############
    nlohmann::ordered_json OptionsToJSON(const Option_Values_Byte_193& byte_193_option_values, const Option_Values_Byte_194& byte_194_option_values, const Option_Values_Byte_195& byte_195_option_values) {
        nlohmann::ordered_json j;

        j["Reserved (Byte 193, Bit 7)"] = byte_193_option_values.reserved_bit_7;
        j["LPMode/TxDis input signal configurable (Byte 193, Bit 6)"] = byte_193_option_values.lpmode_txdis_input_configurable_bit_6;
        j["IntL/RxLOSL output signal configurable (Byte 193, Bit 5)"] = byte_193_option_values.intl_rxlosl_output_configurable_bit_5;
        j["Tx input adaptive equalizers freeze capable (Byte 193, Bit 4)"] = byte_193_option_values.tx_input_adaptive_equalizers_freeze_capable_bit_4;
        j["Tx input equalizers auto-adaptive capable (Byte 193, Bit 3)"] = byte_193_option_values.tx_input_equalizers_auto_adaptive_capable_bit_3;
        j["Tx input equalizers fixed-programmable settings implemented (Byte 193, Bit 2)"] = byte_193_option_values.tx_input_equalizers_fixed_programmable_settings_bit_2;
        j["Rx output emphasis fixed-programmable settings implemented (Byte 193, Bit 1)"] = byte_193_option_values.rx_output_emphasis_fixed_programmable_settings_bit_1;
        j["Rx output amplitude fixed-programmable settings implemented (Byte 193, Bit 0)"] = byte_193_option_values.rx_output_amplitude_fixed_programmable_settings_bit_0;

        j["Tx CDR On/Off Control implemented (Byte 194, Bit 7)"] = byte_194_option_values.tx_cdr_on_off_control_implemented_bit_7;
        j["Rx CDR On/Off Control implemented (Byte 194, Bit 6)"] = byte_194_option_values.rx_cdr_on_off_control_implemented_bit_6;
        j["Tx CDR Loss of Lock (LOL) flag implemented (Byte 194, Bit 5)"] = byte_194_option_values.tx_cdr_loss_of_lock_lol_flag_implemented_bit_5;
        j["Rx CDR Loss of Lock (LOL) flag implemented. (Byte 194, Bit 4)"] = byte_194_option_values.rx_cdr_loss_of_lock_lol_flag_implemented_bit_4;
        j["Rx Squelch Disable implemented (Byte 194, Bit 3)"] = byte_194_option_values.rx_squelch_disable_implemented_bit_3;
        j["Rx Output Disable implemented (Byte 194, Bit 2)"] = byte_194_option_values.rx_output_disable_implemented_bit_2;
        j["Tx Squelch Disable implemented (Byte 194, Bit 1)"] = byte_194_option_values.tx_squelch_disable_implemented_bit_1;
        j["Tx Squelch implemented (Byte 194, Bit 0)"] = byte_194_option_values.tx_squelch_implemented_bit_0;

        j["Memory Page 02 provided (Byte 195, Bit 7)"] = byte_195_option_values.memory_page_02_provided_bit_7;
        j["Memory Page 01h provided. (Byte 195, Bit 6)"] = byte_195_option_values.memory_page_01h_provided_bit_6;
        j["Rate select is implemented as defined in Section 6.2.7 (Byte 195, Bit 5)"] = byte_195_option_values.rate_select_implemented_bit_5;
        j["Tx_Disable is implemented (Byte 195, Bit 4)"] = byte_195_option_values.tx_disable_implemented_bit_4;
        j["Tx_Fault signal implemented (Byte 195, Bit 3)"] = byte_195_option_values.tx_fault_signal_implemented_bit_3;
        j["Tx Squelch implemented to reduce Pave (Byte 195, Bit 2)"] = byte_195_option_values.tx_squelch_implemented_to_reduce_pave_bit_2;
        j["Tx Loss of Signal (Byte 195, Bit 1)"] = byte_195_option_values.tx_loss_of_signal_implemented_bit_1;
        j["Pages 20-21h implemented (Byte 195, Bit 0)"] = byte_195_option_values.pages_20_to_21h_implemented_bit_0;

        return j;
    }

    std::tuple<Option_Values_Byte_193, Option_Values_Byte_194, Option_Values_Byte_195> OptionsFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Options must be an object");

        Option_Values_Byte_193 byte_193_option_values;
        Option_Values_Byte_194 byte_194_option_values;
        Option_Values_Byte_195 byte_195_option_values;


        byte_193_option_values.reserved_bit_7 = j.at("Reserved (Byte 193, Bit 7)").template get<bool>();
        byte_193_option_values.lpmode_txdis_input_configurable_bit_6 = j.at("LPMode/TxDis input signal configurable (Byte 193, Bit 6)").template get<bool>();
        byte_193_option_values.intl_rxlosl_output_configurable_bit_5 = j.at("IntL/RxLOSL output signal configurable (Byte 193, Bit 5)").template get<bool>();
        byte_193_option_values.tx_input_adaptive_equalizers_freeze_capable_bit_4 = j.at("Tx input adaptive equalizers freeze capable (Byte 193, Bit 4)").template get<bool>();
        byte_193_option_values.tx_input_equalizers_auto_adaptive_capable_bit_3 = j.at("Tx input equalizers auto-adaptive capable (Byte 193, Bit 3)").template get<bool>();
        byte_193_option_values.tx_input_equalizers_fixed_programmable_settings_bit_2 = j.at("Tx input equalizers fixed-programmable settings implemented (Byte 193, Bit 2)").template get<bool>();
        byte_193_option_values.rx_output_emphasis_fixed_programmable_settings_bit_1 = j.at("Rx output emphasis fixed-programmable settings implemented (Byte 193, Bit 1)").template get<bool>();
        byte_193_option_values.rx_output_amplitude_fixed_programmable_settings_bit_0 = j.at("Rx output amplitude fixed-programmable settings implemented (Byte 193, Bit 0)").template get<bool>();

        byte_194_option_values.tx_cdr_on_off_control_implemented_bit_7 = j.at("Tx CDR On/Off Control implemented (Byte 194, Bit 7)").template get<bool>();
        byte_194_option_values.rx_cdr_on_off_control_implemented_bit_6 = j.at("Rx CDR On/Off Control implemented (Byte 194, Bit 6)").template get<bool>();
        byte_194_option_values.tx_cdr_loss_of_lock_lol_flag_implemented_bit_5 = j.at("Tx CDR Loss of Lock (LOL) flag implemented (Byte 194, Bit 5)").template get<bool>();
        byte_194_option_values.rx_cdr_loss_of_lock_lol_flag_implemented_bit_4 = j.at("Rx CDR Loss of Lock (LOL) flag implemented. (Byte 194, Bit 4)").template get<bool>();
        byte_194_option_values.rx_squelch_disable_implemented_bit_3 = j.at("Rx Squelch Disable implemented (Byte 194, Bit 3)").template get<bool>();
        byte_194_option_values.rx_output_disable_implemented_bit_2 = j.at("Rx Output Disable implemented (Byte 194, Bit 2)").template get<bool>();
        byte_194_option_values.tx_squelch_disable_implemented_bit_1 = j.at("Tx Squelch Disable implemented (Byte 194, Bit 1)").template get<bool>();
        byte_194_option_values.tx_squelch_implemented_bit_0 = j.at("Tx Squelch implemented (Byte 194, Bit 0)").template get<bool>();

        byte_195_option_values.memory_page_02_provided_bit_7 = j.at("Memory Page 02 provided (Byte 195, Bit 7)").template get<bool>();
        byte_195_option_values.memory_page_01h_provided_bit_6 = j.at("Memory Page 01h provided. (Byte 195, Bit 6)").template get<bool>();
        byte_195_option_values.rate_select_implemented_bit_5 = j.at("Rate select is implemented as defined in Section 6.2.7 (Byte 195, Bit 5)").template get<bool>();
        byte_195_option_values.tx_disable_implemented_bit_4 = j.at("Tx_Disable is implemented (Byte 195, Bit 4)").template get<bool>();
        byte_195_option_values.tx_fault_signal_implemented_bit_3 = j.at("Tx_Fault signal implemented (Byte 195, Bit 3)").template get<bool>();
        byte_195_option_values.tx_squelch_implemented_to_reduce_pave_bit_2 = j.at("Tx Squelch implemented to reduce Pave (Byte 195, Bit 2)").template get<bool>();
        byte_195_option_values.tx_loss_of_signal_implemented_bit_1 = j.at("Tx Loss of Signal (Byte 195, Bit 1)").template get<bool>();
        byte_195_option_values.pages_20_to_21h_implemented_bit_0 = j.at("Pages 20-21h implemented (Byte 195, Bit 0)").template get<bool>();

        return std::make_tuple(std::move(byte_193_option_values), std::move(byte_194_option_values), std::move(byte_195_option_values));
    }

//############

//############
    nlohmann::ordered_json VendorSNToJSON(const std::array<unsigned char, 16>& vendorSNRaw) {
        bool vendorSNPrintable = std::all_of(vendorSNRaw.begin(), vendorSNRaw.end(), [](char c) {return std::isprint(c); });

        nlohmann::ordered_json j;

    
        if(vendorSNPrintable) {
            std::string vendorSN = std::string(reinterpret_cast<char const *>(vendorSNRaw.data()), vendorSNRaw.size());
            //rtrim
            vendorSN.erase(std::find_if(vendorSN.rbegin(), vendorSN.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorSN.end());

            j = vendorSN;
        } else {
            j["Type"] = "Base64";
            j["Value"] = cppcodec::base64_rfc4648::encode(vendorSNRaw.data(), vendorSNRaw.size());
        }

        return j;
    }

    std::array<unsigned char, 16> VendorSNFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 16> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();
            //We don't care whether it contains unprintable characters - when we serialise and check, it's only to prevent invalid JSON being generated
            //However, as this is a parsed value, we know that the JSON must be valid. Let the user enter what they want...
            if(strVal.size() > 16) throw std::invalid_argument("Vendor Serial Number must not have more than 16 characters");

            if(strVal.size() < 16) strVal.resize(16, 0x20);

            std::memcpy(arr.data(), strVal.data(), strVal.size());
        } else if(j.is_object()) {
            if(j.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Serial Number object must have Type Base64");

            auto encodedVal = j.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 16) throw std::invalid_argument("Vendor Serial Number specified as base64 must have exactly 16 characters!");

            std::memcpy(arr.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Serial Number has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json DateCodeToJSON(const DateCode& dateCode) {
        nlohmann::ordered_json j;

        bool dateCodeYearIsNumber = std::isdigit(dateCode.year_low_order_digits[0]) && std::isdigit(dateCode.year_low_order_digits[1]);
        
        if(dateCodeYearIsNumber) {
            int digits_of_year = std::stoi(std::string(reinterpret_cast<char const *>(dateCode.year_low_order_digits.data()), dateCode.year_low_order_digits.size()));

            j["Year low order digits (Byte 212-213)"] = (unsigned)(digits_of_year);
        } else {
            j["Year low order digits (Byte 212-213)"]["Type"] = "Base64";
            j["Year low order digits (Byte 212-213)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.year_low_order_digits.data(), dateCode.year_low_order_digits.size());
        }
        

        bool dateCodeMonthIsNumber = std::isdigit(dateCode.month_digits[0]) && std::isdigit(dateCode.month_digits[1]);

        if(dateCodeMonthIsNumber) {
            int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(dateCode.month_digits.data()), dateCode.month_digits.size()));

            if(digits_of_month < 1 || digits_of_month > 12) {
                j["Month digits (Byte 214-215)"]["Type"] = "Base64";
                j["Month digits (Byte 214-215)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.month_digits.data(), dateCode.month_digits.size());
            } else {
                j["Month digits (Byte 214-215)"] = (unsigned)(digits_of_month);
            }
        } else {
            j["Month digits (Byte 214-215)"]["Type"] = "Base64";
            j["Month digits (Byte 214-215)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.month_digits.data(), dateCode.month_digits.size());
        }

        bool dateCodeDayIsNumber = std::isdigit(dateCode.day_digits[0]) && std::isdigit(dateCode.day_digits[1]);

        if(dateCodeDayIsNumber) {
            int day  = std::stoi(std::string(reinterpret_cast<char const *>(dateCode.day_digits.data()), dateCode.day_digits.size()));

            if(day < 1 || day > 31) {
                j["Day (Byte 216-217)"]["Type"] = "Base64";
                j["Day (Byte 216-217)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.day_digits.data(), dateCode.day_digits.size());
            } else {
                j["Day (Byte 216-217)"] = (unsigned)(day);
            }
        } else {
            j["Day (Byte 216-217)"]["Type"] = "Base64";
            j["Day (Byte 216-217)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.day_digits.data(), dateCode.day_digits.size());
        }

        bool vendorLotCodePrintable = std::isprint(dateCode.lot_code[0]) && std::isprint(dateCode.lot_code[1]);
        if(vendorLotCodePrintable) {
            
            std::string vendorLotCode = std::string(reinterpret_cast<char const *>(dateCode.lot_code.data()), dateCode.lot_code.size());
            vendorLotCode.erase(std::find_if(vendorLotCode.rbegin(), vendorLotCode.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorLotCode.end());

            j["Vendor Lot Code (Byte 218-219)"] = vendorLotCode;
        } else {
            j["Vendor Lot Code (Byte 218-219)"]["Type"] = "Base64";
            j["Vendor Lot Code (Byte 218-219)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.lot_code.data(), dateCode.lot_code.size());
        }
        
        return j;
    }

    DateCode DateCodeFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Date Code must be an object");

        DateCode dateCode;

        auto dateCodeYearVal = j.at("Year low order digits (Byte 212-213)");
        if(dateCodeYearVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeYearVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Year low order digits (Byte 212-213) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.year_low_order_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeYearVal.is_object()) {
            if(dateCodeYearVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Year low order digits (Byte 212-213) object must have Type Base64");

            auto encodedVal = dateCodeYearVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Year low order digits (Byte 212-213) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.year_low_order_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Year low order digits (Byte 212-213) must either be a number or object");
        }

        auto dateCodeMonthVal = j.at("Month digits (Byte 214-215)");
        if(dateCodeMonthVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeMonthVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Month digits (Byte 214-215) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.month_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeMonthVal.is_object()) {
            if(dateCodeMonthVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Month digits (Byte 214-215) object must have Type Base64");

            auto encodedVal = dateCodeMonthVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Month digits (Byte 214-215) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.month_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Month digits (Byte 214-215) must either be a number or object");
        }

        auto dateCodeDayVal = j.at("Day (Byte 216-217)");
        if(dateCodeDayVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeDayVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Day (Byte 216-217) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.day_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeDayVal.is_object()) {
            if(dateCodeDayVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Day (Byte 216-217) object must have Type Base64");

            auto encodedVal = dateCodeDayVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Day (Byte 216-217) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.day_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Day (Byte 216-217) must either be a number or object");
        }

        auto dateCodeLotVal = j.at("Vendor Lot Code (Byte 218-219)");
        if(dateCodeLotVal.is_string()) {
            auto dateCodeStrVal = dateCodeLotVal.template get<std::string>();
            if(dateCodeStrVal.size() > 2) throw std::invalid_argument("Vendor Lot Code (Byte 218-219) string must not be longer than 2 characters");
            if(dateCodeStrVal.size() < 2) dateCodeStrVal.resize(2, 0x20);
            
            //We don't care if it's not a valid number... Could as well have been specified as base64
            std::memcpy(dateCode.lot_code.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeLotVal.is_object()) {
            if(dateCodeLotVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Lot Code (Byte 218-219) object must have Type Base64");

            auto encodedVal = dateCodeLotVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Vendor Lot Code (Byte 218-219) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.lot_code.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Lot Code (Byte 218-219) must either be a string or object");
        }

        return dateCode;
    }
//############

//############

    nlohmann::ordered_json Diagnostic_Monitoring_TypeToJSON(const Diagnostic_Monitoring_Type& value) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Reserved (Bit 6)"] = value.reserved_bit_6;
        j["Temperature monitoring implemented (Bit 5)"] = value.temperature_monitoring_implemented_bit_5;
        j["Supply voltage monitoring implemented (Bit 4)"] = value.supply_voltage_monitoring_implemented_bit_4;
        j["Received power measurements type (Bit 3)"] = value.received_power_measurement_is_average_bit_3 ? "Average": "OMA";
        j["Transmitter power measurement supported (Bit 2)"] = value.transmitter_power_measurement_supported_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["Reserved (Bit 0)"] = value.reserved_bit_0;

        return j;
    }

    Diagnostic_Monitoring_Type Diagnostic_Monitoring_TypeFromJSON(const nlohmann::json& j) {
        Diagnostic_Monitoring_Type monitoringType;

        monitoringType.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        monitoringType.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        monitoringType.temperature_monitoring_implemented_bit_5 = j.at("Temperature monitoring implemented (Bit 5)").template get<bool>();
        monitoringType.supply_voltage_monitoring_implemented_bit_4 = j.at("Supply voltage monitoring implemented (Bit 4)").template get<bool>();

        auto powerMeasurementStr = j.at("Received power measurements type (Bit 3)").template get<std::string>();
        if(powerMeasurementStr == "Average") {
            monitoringType.received_power_measurement_is_average_bit_3 = true;
        } else if(powerMeasurementStr == "OMA") {
            monitoringType.received_power_measurement_is_average_bit_3 = false;
        } else {
            throw std::invalid_argument("Received power measurements type (Bit 3) string has invalid value, must be either Average or OMA");
        }

        monitoringType.transmitter_power_measurement_supported_bit_2 = j.at("Transmitter power measurement supported (Bit 2)").template get<bool>();
        monitoringType.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        monitoringType.reserved_bit_0 = j.at("Reserved (Bit 0)").template get<bool>();

        return monitoringType;
    }

//############

//############
    nlohmann::ordered_json Enhanced_OptionsToJSON(const Enhanced_Options& value) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Reserved (Bit 6)"] = value.reserved_bit_6;
        j["Reserved (Bit 5)"] = value.reserved_bit_5;
        j["Initialization Complete Flag implemented (Bit 4)"] = value.initialization_complete_flag_implemented_bit_4;
        j["Extended Rate Selection implemented (Bit 3)"] = value.rate_selection_is_implemented_using_extended_rate_selection_bit_3;
        j["Reserved (Bit 2)"] = value.reserved_bit_2;
        j["TC readiness flag implemented (Bit 1)"] = value.readiness_flag_implemented_bit_1;
        j["Software reset implemented (Bit 0)"] = value.software_reset_implemented_bit_0;

        return j;
    }

    Enhanced_Options Enhanced_OptionsFromJSON(const nlohmann::json& j) {
        Enhanced_Options enhancedOptions;

        enhancedOptions.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        enhancedOptions.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        enhancedOptions.reserved_bit_5 = j.at("Reserved (Bit 5)").template get<bool>();
        enhancedOptions.initialization_complete_flag_implemented_bit_4 = j.at("Initialization Complete Flag implemented (Bit 4)").template get<bool>();
        enhancedOptions.rate_selection_is_implemented_using_extended_rate_selection_bit_3 = j["Extended Rate Selection implemented (Bit 3)"];
        enhancedOptions.reserved_bit_2 = j.at("Reserved (Bit 2)").template get<bool>();
        enhancedOptions.readiness_flag_implemented_bit_1 = j.at("TC readiness flag implemented (Bit 1)").template get<bool>();
        enhancedOptions.software_reset_implemented_bit_0 = j.at("Software reset implemented (Bit 0)").template get<bool>();

        return enhancedOptions;
    }
//############

//############
    nlohmann::ordered_json ExtendedBaudRate250MBaudToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        j = (unsigned long)(byte_value) * 250ul;

        return j;
    }

    unsigned char ExtendedBaudRate250MBaudFromJSON(const nlohmann::json& j) {
        auto numberValue = j.template get<std::uint64_t>();

        if(numberValue % 250 != 0) throw std::invalid_argument("Extended Baud Rate must be divisble by 250");

        if(numberValue > 63750) throw std::invalid_argument("Extended Baud Rate must not be larger than 25400");

        return numberValue / 250;
    }
//############

//############
    nlohmann::ordered_json VendorSpecificToJSON(const std::array<unsigned char, 32>& vendorSpecificRaw) {
        nlohmann::ordered_json j;

        j["Type"] = "Base64";
        j["Value"] = cppcodec::base64_rfc4648::encode(vendorSpecificRaw.data(), vendorSpecificRaw.size());

        return j;
    }

    std::array<unsigned char, 32> VendorSpecificFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Vendor Specific must be a base64 object");

        std::array<unsigned char, 32> arr;

        if(j.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Specific object must have Type Base64");

        auto encodedVal = j.at("Value").template get<std::string>();

        std::vector<uint8_t> decoded;
        try {
            decoded = cppcodec::base64_rfc4648::decode(encodedVal);
        } catch(const cppcodec::parse_error& e) {
            throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
        }
        if(decoded.size() != 32) throw std::invalid_argument("Vendor Specific specified as base64 must have exactly 32 characters!");

        std::memcpy(arr.data(), decoded.data(), decoded.size());


        return arr;
    }
//############

    void SFF8636_Upper00hToJSON(nlohmann::ordered_json& j, const SFF8636_Upper00h& programming, bool copperMode) {

        j["Type"] = "SFF-8636 Rev 2.11 Upper Page 00h";
        
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

        j["Encoding"] = EncodingToJSON(programming.byte_139_Encoding);

        j["Nominal Signaling Rate [MBaud] (Divisible by 100)"] = NominalSignalingRate100MBaudToJSON(programming.byte_140_nominal_signaling_rate_in_100_mbaud);

        j["Extended Rate Select Compliance"] = Extended_Rate_Select_ComplianceToJSON(programming.byte_141_extended_rate_select_compliance);

        j["Length (Standard SM Fiber) [km]"] = LengthSMFkmToJSON(programming.byte_142_length_smf_in_kilometers);

        j["Length (OM3 50 um) [m] (Divisible by 2)"] = LengthOM3mToJSON(programming.byte_143_length_om3_in_2m);

        j["Length (OM2 50 um) [m]"] = LengthOM2mToJSON(programming.byte_144_length_om2_in_m);

        j["Copper or Fibre Properties"] = CopperOrFibreInfoToJSON(
            copperMode,
            programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB,
            programming.byte_146_length_copper_in_1m_or_om4_in_2m,
            programming.byte_186_wavelength_high_order_or_copper_attenuation,
            programming.byte_187_wavelength_low_order_or_copper_attenuation,
            programming.byte_188_wavelength_tolerance_high_order_or_copper_attenuation,
            programming.byte_189_wavelength_tolerance_low_order_or_copper_attenuation
        );

        j["Device & Transmitter Properties"] = Device_Technology_and_Transmitter_TechnologyToJSON(programming.byte_147_device_technology_and_transmitter_technology);

        j["Vendor Name"] = VendorNameToJSON(programming.byte_148_163_vendor_name);

        j["Extended Module Codes (Infiniband)"] = Extended_Module_CodesToJSON(programming.byte_164_extended_module_codes);

        j["Vendor OUI"] = VendorOUIToJSON(programming.byte_165_167_vendor_oui);

        j["Vendor Part Number"] = VendorPNToJSON(programming.byte_168_183_vendor_pn);

        j["Vendor Rev"] = VendorRevToJSON(programming.byte_184_185_vendor_rev);

        j["Maximum Case Temperature [degC]"] = MaxCaseTemperatureToJSON(programming.byte_190_max_case_temperature);

        //FIXME: CC_BASE

        j["Extended Specification Compliance Codes"] = ExtendedComplianceCodesToJSON(programming.byte_192_extended_specification_compliance_codes);

        j["Options"] = OptionsToJSON(programming.byte_193_option_values, programming.byte_194_option_values, programming.byte_195_option_values);

        j["Vendor Serial Number"] = VendorSNToJSON(programming.byte_196_211_vendor_sn);

        j["Date Code"] = DateCodeToJSON(programming.byte_212_219_date_code);

        j["Diagnostic Monitoring Type"] = Diagnostic_Monitoring_TypeToJSON(programming.byte_220_diagnostic_monitoring_type);

        j["Enhanced Options"] = Enhanced_OptionsToJSON(programming.byte_221_enhanced_options);

        j["Extended Baud Rate [MBaud] (Divisible by 250)"] = ExtendedBaudRate250MBaudToJSON(programming.byte_222_extended_baud_rate_in_250_mbaud);

        //FIXME: CC_EXT

        j["Vendor Specific"] = VendorSpecificToJSON(programming.byte_224_255_vendor_specific);
    }


    void SFF8636_Upper00hFromJSON(const nlohmann::json& j, SFF8636_Upper00h& programming) {

        if(j.at("Type").template get<std::string>() != "SFF-8636 Rev 2.11 Upper Page 00h") {
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

        programming.byte_139_Encoding = EncodingFromJSON(j.at("Encoding"));

        programming.byte_140_nominal_signaling_rate_in_100_mbaud = NominalSignalingRate100MBaudFromJSON(j.at("Nominal Signaling Rate [MBaud] (Divisible by 100)"));

        programming.byte_141_extended_rate_select_compliance = Extended_Rate_Select_ComplianceJSON(j.at("Extended Rate Select Compliance"));

        programming.byte_142_length_smf_in_kilometers = LengthSMFkmFromJSON(j.at("Length (Standard SM Fiber) [km]"));

        programming.byte_143_length_om3_in_2m = LengthOM3mFromJSON(j.at("Length (OM3 50 um) [m] (Divisible by 2)"));

        programming.byte_144_length_om2_in_m = LengthOM2mFromJSON(j.at("Length (OM2 50 um) [m]"));

        auto copperOrFibreInfoToJSONReturn = CopperOrFibreInfoFromJSON(j.at("Copper or Fibre Properties"));
        programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = copperOrFibreInfoToJSONReturn.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB;
        programming.byte_146_length_copper_in_1m_or_om4_in_2m = copperOrFibreInfoToJSONReturn.byte_146_length_copper_in_1m_or_om4_in_2m;
        programming.byte_186_wavelength_high_order_or_copper_attenuation = copperOrFibreInfoToJSONReturn.byte_186_wavelength_high_order_or_copper_attenuation;
        programming.byte_187_wavelength_low_order_or_copper_attenuation = copperOrFibreInfoToJSONReturn.byte_187_wavelength_low_order_or_copper_attenuation;
        programming.byte_188_wavelength_tolerance_high_order_or_copper_attenuation = copperOrFibreInfoToJSONReturn.byte_188_wavelength_tolerance_high_order_or_copper_attenuation;
        programming.byte_189_wavelength_tolerance_low_order_or_copper_attenuation = copperOrFibreInfoToJSONReturn.byte_189_wavelength_tolerance_low_order_or_copper_attenuation;

        programming.byte_147_device_technology_and_transmitter_technology = Device_Technology_and_Transmitter_TechnologyFromJSON(j.at("Device & Transmitter Properties"));

        programming.byte_148_163_vendor_name = VendorNameFromJSON(j.at("Vendor Name"));

        programming.byte_164_extended_module_codes = Extended_Module_CodesFromJSON(j.at("Extended Module Codes (Infiniband)"));

        programming.byte_165_167_vendor_oui = VendorOUIFromJSON(j.at("Vendor OUI"));

        programming.byte_168_183_vendor_pn = VendorPNFromJSON(j.at("Vendor Part Number"));

        programming.byte_184_185_vendor_rev = VendorRevFromJSON(j.at("Vendor Rev"));

        programming.byte_190_max_case_temperature = MaxCaseTemperatureFromJSON(j.at("Maximum Case Temperature [degC]"));

        //FIXME: CC_BASE

        programming.byte_192_extended_specification_compliance_codes = ExtendedComplianceCodesFromJSON(j.at("Extended Specification Compliance Codes"));

        std::tie(programming.byte_193_option_values, programming.byte_194_option_values, programming.byte_195_option_values) =
            OptionsFromJSON(j.at("Options"));

        programming.byte_196_211_vendor_sn = VendorSNFromJSON(j.at("Vendor Serial Number"));

        programming.byte_212_219_date_code = DateCodeFromJSON(j.at("Date Code"));

        programming.byte_220_diagnostic_monitoring_type = Diagnostic_Monitoring_TypeFromJSON(j.at("Diagnostic Monitoring Type"));

        programming.byte_221_enhanced_options = Enhanced_OptionsFromJSON(j.at("Enhanced Options"));

        programming.byte_222_extended_baud_rate_in_250_mbaud = ExtendedBaudRate250MBaudFromJSON(j.at("Extended Baud Rate [MBaud] (Divisible by 250)"));

        //FIXME: CC_EXT

        programming.byte_224_255_vendor_specific = VendorSpecificFromJSON(j.at("Vendor Specific"));
    }   


}