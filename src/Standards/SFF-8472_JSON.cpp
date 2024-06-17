#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include <fmt/core.h>


namespace TransceiverTool::Standards::SFF8472 {

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
    nlohmann::ordered_json PhysicalDeviceIdentifierToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8472::PhysicalDeviceIdentifierAssignedValues.begin(),
            SFF8472::PhysicalDeviceIdentifierAssignedValues.end(),
            [byte_value](const SFF8472::PhysicalDeviceIdentifierAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8472::PhysicalDeviceIdentifierAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char PhysicalDeviceIdentifierFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8472::PhysicalDeviceIdentifierAssignedValues.begin(),
                SFF8472::PhysicalDeviceIdentifierAssignedValues.end(),
                [&strValue](const SFF8472::PhysicalDeviceIdentifierAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8472::PhysicalDeviceIdentifierAssignedValues.end()) throw std::invalid_argument("Identifier is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Identifier has wrong type (neither string nor object)");
        }
    }
//############

//############
    nlohmann::ordered_json PhysicalDeviceExtendedIdentifierToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.begin(),
            SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.end(),
            [byte_value](const SFF8472::PhysicalDeviceExtendedIdentifierAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char PhysicalDeviceExtendedIdentifierFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.begin(),
                SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.end(),
                [&strValue](const SFF8472::PhysicalDeviceExtendedIdentifierAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues.end()) throw std::invalid_argument("Extended Identifier is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Extended Identifier has wrong type (neither string nor object)");
        }
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
    nlohmann::ordered_json _10G_Ethernet_Compliance_CodesToJSON(const _10G_Ethernet_Compliance_Codes& value) {
        nlohmann::ordered_json j;

        j["10GBASE-ER compliant (Bit 7)"] = value._10GBASE_ER_bit_7;
        j["10GBASE-LRM compliant (Bit 6)"] = value._10GBASE_LRM_bit_6;
        j["10GBASE-LR compliant (Bit 5)"] = value._10GBASE_LR_bit_5;
        j["10GBASE-SR compliant (Bit 4)"] = value._10GBASE_SR_bit_4;

        return j;
    }

    _10G_Ethernet_Compliance_Codes _10G_Ethernet_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("10G Ethernet Compliance Codes must be an object");

        _10G_Ethernet_Compliance_Codes complianceCodes;

        complianceCodes._10GBASE_ER_bit_7 = j.at("10GBASE-ER compliant (Bit 7)").template get<bool>();
        complianceCodes._10GBASE_LRM_bit_6 = j.at("10GBASE-LRM compliant (Bit 6)").template get<bool>();
        complianceCodes._10GBASE_LR_bit_5 = j.at("10GBASE-LR compliant (Bit 5)").template get<bool>();
        complianceCodes._10GBASE_SR_bit_4 = j.at("10GBASE-SR compliant (Bit 4)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json Infiniband_Compliance_CodesToJSON(const Infiniband_Compliance_Codes& value) {
        nlohmann::ordered_json j;

        j["1X SX compliant (Bit 3)"] = value._1X_SX_bit_3;
        j["1X LX compliant (Bit 2)"] = value._1X_LX_bit_2;
        j["1X Copper Active compliant (Bit 1)"] = value._1X_Copper_Active_bit_1;
        j["1X Copper Passive compliant (Bit 0)"] = value._1X_Copper_Passive_bit_0;

        return j;
    }

    Infiniband_Compliance_Codes Infiniband_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Infiniband Compliance Codes must be an object");

        Infiniband_Compliance_Codes complianceCodes;

        complianceCodes._1X_SX_bit_3 = j.at("1X SX compliant (Bit 3)").template get<bool>();
        complianceCodes._1X_LX_bit_2 = j.at("1X LX compliant (Bit 2)").template get<bool>();
        complianceCodes._1X_Copper_Active_bit_1 = j.at("1X Copper Active compliant (Bit 1)").template get<bool>();
        complianceCodes._1X_Copper_Passive_bit_0 = j.at("1X Copper Passive compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json ESCON_Compliance_CodesToJSON(const ESCON_Compliance_Codes& value) {
        nlohmann::ordered_json j;

        j["ESCON MMF, 1310nm LED compliant (Bit 7)"] = value.ESCON_MMF_1310nm_LED_bit_7;
        j["ESCON SMF, 1310nm Laser compliant (Bit 6)"] = value.ESCON_SMF_1310nm_Laser_bit_6;

        return j;
    }

    ESCON_Compliance_Codes ESCON_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("ESCON Compliance Codes must be an object");

        ESCON_Compliance_Codes complianceCodes;

        complianceCodes.ESCON_MMF_1310nm_LED_bit_7 = j.at("ESCON MMF, 1310nm LED compliant (Bit 7)").template get<bool>();
        complianceCodes.ESCON_SMF_1310nm_Laser_bit_6 = j.at("ESCON SMF, 1310nm Laser compliant (Bit 6)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json SONET_Compliance_CodesToJSON(const SONET_Compliance_Codes& value) {
        nlohmann::ordered_json j;

        j["OC-192, short reach compliant (Byte 4, Bit 5)"] = value.OC_192_short_reach_byte_4_bit_5;
        j["SONET reach specifier (Byte 4, Bit 4-3)"] = getSFF8472_SONETReachSpecifierInfo(value.SONET_Reach_Specifier_byte_4_bit_4_3).name;
        j["OC-48, long reach compliant (Byte 4, Bit 2)"] = value.OC_48_long_reach_byte_4_bit_2;
        j["OC-48, intermediate reach compliant (Byte 4, Bit 1)"] = value.OC_48_intermediate_reach_byte_4_bit_1;
        j["OC-48, short reach compliant (Byte 4, Bit 0)"] = value.OC_48_short_reach_byte_4_bit_0;
        j["Reserved (Byte 5, Bit 7)"] = value.reserved_byte_5_bit_7;
        j["OC-12, single mode, long reach compliant (Byte 5, Bit 6)"] = value.OC_12_single_mode_long_reach_2_byte_5_bit_6;
        j["OC-12, single mode, intermediate reach compliant (Byte 5, Bit 5)"] = value.OC_12_single_mode_intermediate_reach_byte_5_bit_5;
        j["OC-12, short reach compliant (Byte 5, Bit 4)"] = value.OC_12_short_reach_byte_5_bit_4;
        j["Reserved (Byte 5, Bit 3)"] = value.reserved_byte_5_bit_3;
        j["OC-3, single mode, long reach compliant (Byte 5, Bit 2)"] = value.OC_3_single_mode_long_reach_byte_5_bit_2;
        j["OC-3, single mode, intermediate reach compliant (Byte 5, Bit 1)"] = value.OC_3_single_mode_intermediate_reach_byte_5_bit_1;
        j["OC-3, short reach compliant (Byte 5, Bit 0)"] = value.OC_3_short_reach__byte_5_bit_0;

        

        return j;
    }

    SONET_Compliance_Codes SONET_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("SONET Compliance Codes must be an object");

        SONET_Compliance_Codes complianceCodes;

        complianceCodes.OC_192_short_reach_byte_4_bit_5 = j.at("OC-192, short reach compliant (Byte 4, Bit 5)").template get<bool>();

        auto reachSpecifierStr = j.at("SONET reach specifier (Byte 4, Bit 4-3)").template get<std::string>();
        auto it = std::find_if(
            SONETReachSpecifierAssignedValues.begin(),
            SONETReachSpecifierAssignedValues.end(),
            [&reachSpecifierStr](const SONETReachSpecifierAssignedValue& entry) { return entry.name == reachSpecifierStr; }
        );

        if(it == SONETReachSpecifierAssignedValues.end()) throw std::invalid_argument("SONET reach specifier is not a known string value");

        complianceCodes.SONET_Reach_Specifier_byte_4_bit_4_3 = it->enum_value;

        complianceCodes.OC_48_long_reach_byte_4_bit_2 = j.at("OC-48, long reach compliant (Byte 4, Bit 2)").template get<bool>();
        complianceCodes.OC_48_intermediate_reach_byte_4_bit_1 = j.at("OC-48, intermediate reach compliant (Byte 4, Bit 1)").template get<bool>();
        complianceCodes.OC_48_short_reach_byte_4_bit_0 = j.at("OC-48, short reach compliant (Byte 4, Bit 0)").template get<bool>();
        complianceCodes.reserved_byte_5_bit_7 = j.at("Reserved (Byte 5, Bit 7)").template get<bool>();
        complianceCodes.OC_12_single_mode_long_reach_2_byte_5_bit_6 = j.at("OC-12, single mode, long reach compliant (Byte 5, Bit 6)").template get<bool>();
        complianceCodes.OC_12_single_mode_intermediate_reach_byte_5_bit_5 = j.at("OC-12, single mode, intermediate reach compliant (Byte 5, Bit 5)").template get<bool>();
        complianceCodes.OC_12_short_reach_byte_5_bit_4 = j.at("OC-12, short reach compliant (Byte 5, Bit 4)").template get<bool>();
        complianceCodes.reserved_byte_5_bit_3 = j.at("Reserved (Byte 5, Bit 3)").template get<bool>();
        complianceCodes.OC_3_single_mode_long_reach_byte_5_bit_2 = j.at("OC-3, single mode, long reach compliant (Byte 5, Bit 2)").template get<bool>();
        complianceCodes.OC_3_single_mode_intermediate_reach_byte_5_bit_1 = j.at("OC-3, single mode, intermediate reach compliant (Byte 5, Bit 1)").template get<bool>();
        complianceCodes.OC_3_short_reach__byte_5_bit_0 = j.at("OC-3, short reach compliant (Byte 5, Bit 0)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json Ethernet_Compliance_CodesToJSON(const Ethernet_Compliance_Codes& value) {
        nlohmann::ordered_json j;

        j["BASE-PX compliant (Bit 7)"] = value.BASE_PX_bit_7;
        j["BASE-BX10 compliant (Bit 6)"] = value.BASE_BX10_bit_6;
        j["100BASE-FX compliant (Bit 5)"] = value._100BASE_FX_bit_5;
        j["100BASE-LX/LX10 compliant (Bit 4)"] = value._100BASE_LX_LX10_bit_4;
        j["1000BASE-T compliant (Bit 3)"] = value._1000BASE_T_bit_3;
        j["1000BASE-CX compliant (Bit 2)"] = value._1000BASE_CX_bit_2;
        j["1000BASE-LX compliant (Bit 1)"] = value._1000BASE_LX_bit_1;
        j["1000BASE-SX compliant (Bit 0)"] = value._1000BASE_SX_bit_0;

        return j;
    }

    Ethernet_Compliance_Codes Ethernet_Compliance_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Ethernet Compliance Codes must be an object");

        Ethernet_Compliance_Codes complianceCodes;

        complianceCodes.BASE_PX_bit_7 = j.at("BASE-PX compliant (Bit 7)").template get<bool>();
        complianceCodes.BASE_BX10_bit_6 = j.at("BASE-BX10 compliant (Bit 6)").template get<bool>();
        complianceCodes._100BASE_FX_bit_5 = j.at("100BASE-FX compliant (Bit 5)").template get<bool>();
        complianceCodes._100BASE_LX_LX10_bit_4 = j.at("100BASE-LX/LX10 compliant (Bit 4)").template get<bool>();
        complianceCodes._1000BASE_T_bit_3 = j.at("1000BASE-T compliant (Bit 3)").template get<bool>();
        complianceCodes._1000BASE_CX_bit_2 = j.at("1000BASE-CX compliant (Bit 2)").template get<bool>();
        complianceCodes._1000BASE_LX_bit_1 = j.at("1000BASE-LX compliant (Bit 1)").template get<bool>();
        complianceCodes._1000BASE_SX_bit_0 = j.at("1000BASE-SX compliant (Bit 0)").template get<bool>();


        return complianceCodes;
    }
//############


//############
    nlohmann::ordered_json Fibre_Channel_Link_Length_CodesToJSON(const Fibre_Channel_Link_Length_Codes& value) {
        nlohmann::ordered_json j;

        j["very long distance (V) compliant (Bit 7)"] = value.very_long_distance_v_bit_7;
        j["short distance (S) compliant (Bit 6)"] = value.short_distance_s_bit_6;
        j["intermediate distance (I) compliant (Bit 5)"] = value.intermediate_distance_i_bit_5;
        j["long distance (L) compliant (Bit 4)"] = value.long_distance_l_bit_4;
        j["medium distance (M) compliant (Bit 3)"] = value.medium_distance_m_bit_3;

        return j;
    }

    Fibre_Channel_Link_Length_Codes Fibre_Channel_Link_Length_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Link Length must be an object");

        Fibre_Channel_Link_Length_Codes complianceCodes;

        complianceCodes.very_long_distance_v_bit_7 = j.at("very long distance (V) compliant (Bit 7)").template get<bool>();
        complianceCodes.short_distance_s_bit_6 = j.at("short distance (S) compliant (Bit 6)").template get<bool>();
        complianceCodes.intermediate_distance_i_bit_5 = j.at("intermediate distance (I) compliant (Bit 5)").template get<bool>();
        complianceCodes.long_distance_l_bit_4 = j.at("long distance (L) compliant (Bit 4)").template get<bool>();
        complianceCodes.medium_distance_m_bit_3 = j.at("medium distance (M) compliant (Bit 3)").template get<bool>();


        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json Fibre_Channel_Technology_CodesToJSON(const Fibre_Channel_Technology_Codes& value) {
        nlohmann::ordered_json j;

        j["Shortwave laser, linear Rx (SA) compliant (Byte 7, Bit 2)"] = value.Shortwave_laser_linear_Rx_SA_byte_7_bit_2;
        j["Longwave laser (LC) compliant (Byte 7, Bit 1)"] = value.Longwave_laser_LC_byte_7_bit_1;
        j["Electrical inter-enclosure (EL) compliant (Byte 7, Bit 0)"] = value.Electrical_inter_enclosure_EL_byte_7_bit_0;
        j["Electrical intra-enclosure (EL) compliant (Byte 8, Bit 7)"] = value.Electrical_intra_enclosure_EL_byte_byte_8_bit_7;
        j["Shortwave laser w/o OFC (SN) compliant (Byte 8, Bit 6)"] = value.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6;
        j["Shortwave laser with OFC (SL) compliant (Byte 8, Bit 5)"] = value.Shortwave_laser_with_OFC_SL_byte_8_bit_5;
        j["Longwave laser (LL) compliant (Byte 8, Bit 4)"] = value.Longwave_Laser_LL_byte_8_bit_4;
        
        return j;
    }

    Fibre_Channel_Technology_Codes Fibre_Channel_Technology_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Fibre_Channel_Technology_Codes complianceCodes;

        complianceCodes.Shortwave_laser_linear_Rx_SA_byte_7_bit_2 = j.at("Shortwave laser, linear Rx (SA) compliant (Byte 7, Bit 2)").template get<bool>();
        complianceCodes.Longwave_laser_LC_byte_7_bit_1 = j.at("Longwave laser (LC) compliant (Byte 7, Bit 1)").template get<bool>();
        complianceCodes.Electrical_inter_enclosure_EL_byte_7_bit_0 = j.at("Electrical inter-enclosure (EL) compliant (Byte 7, Bit 0)").template get<bool>();
        complianceCodes.Electrical_intra_enclosure_EL_byte_byte_8_bit_7 = j.at("Electrical intra-enclosure (EL) compliant (Byte 8, Bit 7)").template get<bool>();
        complianceCodes.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6 = j.at("Shortwave laser w/o OFC (SN) compliant (Byte 8, Bit 6)").template get<bool>();
        complianceCodes.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6 = j.at("Shortwave laser with OFC (SL) compliant (Byte 8, Bit 5)").template get<bool>();
        complianceCodes.Shortwave_laser_with_OFC_SL_byte_8_bit_5 = j.at("Longwave laser (LL) compliant (Byte 8, Bit 4)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json SFP_plus_Cable_Technology_CodesToJSON(const SFP_plus_Cable_Technology_Codes& value) {
        nlohmann::ordered_json j;

        j["Active Cable compliant (Bit 3)"] = value.Active_Cable_bit_3;
        j["Passive Cable compliant (Bit 2)"] = value.Passive_Cable_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["Reserved (Bit 0)"] = value.reserved_bit_0;

        return j;
    }

    SFP_plus_Cable_Technology_Codes SFP_plus_Cable_Technology_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("SFP+ Cable Technology must be an object");

        SFP_plus_Cable_Technology_Codes complianceCodes;

        complianceCodes.Active_Cable_bit_3 = j.at("Active Cable compliant (Bit 3)").template get<bool>();
        complianceCodes.Passive_Cable_bit_2 = j.at("Passive Cable compliant (Bit 2)").template get<bool>();
        complianceCodes.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        complianceCodes.reserved_bit_0 = j.at("Reserved (Bit 0)").template get<bool>();

        return complianceCodes;
    }
//############

    void SFF8472_LowerA0hToJSON(nlohmann::ordered_json& j, const SFF8472_LowerA0h& programming, bool copperMode) {

        std::vector<unsigned char> binaryBuffer; binaryBuffer.resize(128, 0x00);


        j["Type"] = "SFF-8472 Rev 12.4 Lower Page A0h";
        
        j["Identifier"] = PhysicalDeviceIdentifierToJSON(programming.byte_0_Identifier);

        j["Extended Identifier"] = PhysicalDeviceExtendedIdentifierToJSON(programming.byte_1_extended_identifier);

        j["Connector Type"] = TransceiverConnectorTypeToJSON(programming.byte_2_Connector_type);


        j["10G Ethernet Compliance Codes"] = _10G_Ethernet_Compliance_CodesToJSON(programming.byte_3_ethernet_compliance_codes);
        j["Infiniband Compliance Codes"] = Infiniband_Compliance_CodesToJSON(programming.byte_3_infiniband_compliance_codes);

        j["ESCON Compliance Codes"] = ESCON_Compliance_CodesToJSON(programming.byte_4_escon_compliance_codes);
        j["SONET Compliance Codes"] = SONET_Compliance_CodesToJSON(programming.byte_4_5_sonet_compliance_codes);

        j["Ethernet Compliance Codes"] = Ethernet_Compliance_CodesToJSON(programming.byte_6_ethernet_compliance_codes);

        j["Fibre Channel Link Length"] = Fibre_Channel_Link_Length_CodesToJSON(programming.byte_7_fibre_channel_link_length_codes);
        j["Fibre Channel Technology"] = Fibre_Channel_Technology_CodesToJSON(programming.byte_7_8_fibre_channel_technology_codes);
        j["SFP+ Cable Technology"] = SFP_plus_Cable_Technology_CodesToJSON(programming.byte_8_sfp_plus_cable_technology_codes);
    }



    void SFF8472_LowerA0hFromJSON(const nlohmann::json& j, SFF8472_LowerA0h& programming) {

        if(j.at("Type").template get<std::string>() != "SFF-8472 Rev 12.4 Lower Page A0h") {
            throw std::invalid_argument("JSON specifies wrong type");
        }

        programming.byte_0_Identifier = PhysicalDeviceIdentifierFromJSON(j.at("Identifier"));

        programming.byte_1_extended_identifier = PhysicalDeviceExtendedIdentifierFromJSON(j.at("Extended Identifier"));

        programming.byte_2_Connector_type = TransceiverConnectorTypeFromJSON(j.at("Connector Type"));

        programming.byte_3_ethernet_compliance_codes = _10G_Ethernet_Compliance_CodesFromJSON(j.at("10G Ethernet Compliance Codes"));
        programming.byte_3_infiniband_compliance_codes = Infiniband_Compliance_CodesFromJSON(j.at("Infiniband Compliance Codes"));

        programming.byte_4_escon_compliance_codes = ESCON_Compliance_CodesFromJSON(j.at("ESCON Compliance Codes"));
        programming.byte_4_5_sonet_compliance_codes = SONET_Compliance_CodesFromJSON(j.at("SONET Compliance Codes"));

        programming.byte_6_ethernet_compliance_codes = Ethernet_Compliance_CodesFromJSON(j.at("Ethernet Compliance Codes"));

        programming.byte_7_fibre_channel_link_length_codes = Fibre_Channel_Link_Length_CodesFromJSON(j.at("Fibre Channel Link Length"));
        programming.byte_7_8_fibre_channel_technology_codes = Fibre_Channel_Technology_CodesFromJSON(j.at("Fibre Channel Technology"));
        programming.byte_8_sfp_plus_cable_technology_codes = SFP_plus_Cable_Technology_CodesFromJSON(j.at("SFP+ Cable Technology"));
    }   
}