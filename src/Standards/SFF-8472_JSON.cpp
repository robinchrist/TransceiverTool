#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include "TransceiverTool/Standards/SFF-8024_Encoding_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Date_Code.hpp"
#include "TransceiverTool/Standards/SFF-8472_Diagnostic_Monitoring_Type.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/SFF-8472_Option_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Extended_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Rate_Identifiers.hpp"
#include <fmt/core.h>
#include <optional>
#include <stdexcept>
#include <cppcodec/base64_rfc4648.hpp>
#include "TransceiverTool/Standards/common.hpp"
#include "TransceiverTool/Vendor_OUIs.hpp"


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

//############
    nlohmann::ordered_json Fibre_Channel_Transmission_Media_CodesToJSON(const Fibre_Channel_Transmission_Media_Codes& value) {
        nlohmann::ordered_json j;

        j["Twin Axial Pair (TW) compliant (Bit 7)"] = value.Twin_Axial_Pair_TW_bit_7;
        j["Twisted Pair (TP) compliant (Bit 6)"] = value.Twisted_Pair_TP_bit_6;
        j["Miniature Coax (MI compliant (Bit 5)"] = value.Miniature_Coax_MI_bit_5;
        j["Video Coax (TV) compliant (Bit 4)"] = value.Video_Coax_TV_bit_4;
        j["Multimode, 62.5um (M6) compliant (Bit 3)"] = value.Multimode_62_5_um_M6_bit_3;
        j["Multimode, 50um (M5, M5E) compliant (Bit 2)"] = value.Multimode_50_um_M5_M5E_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["Single Mode (SM) compliant (Bit 0)"] = value.Single_Mode_SM_bit_0;

        return j;
    }

    Fibre_Channel_Transmission_Media_Codes Fibre_Channel_Transmission_Media_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Fibre_Channel_Transmission_Media_Codes complianceCodes;

        complianceCodes.Twin_Axial_Pair_TW_bit_7 = j.at("Twin Axial Pair (TW) compliant (Bit 7)").template get<bool>();
        complianceCodes.Twisted_Pair_TP_bit_6 = j.at("Twisted Pair (TP) compliant (Bit 6)").template get<bool>();
        complianceCodes.Miniature_Coax_MI_bit_5 = j.at("Miniature Coax (MI compliant (Bit 5)").template get<bool>();
        complianceCodes.Video_Coax_TV_bit_4 = j.at("Video Coax (TV) compliant (Bit 4)").template get<bool>();
        complianceCodes.Multimode_62_5_um_M6_bit_3 = j.at("Multimode, 62.5um (M6) compliant (Bit 3)").template get<bool>();
        complianceCodes.Multimode_50_um_M5_M5E_bit_2 = j.at("Multimode, 50um (M5, M5E) compliant (Bit 2)").template get<bool>();
        complianceCodes.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        complianceCodes.Single_Mode_SM_bit_0 = j.at("Single Mode (SM) compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json Fibre_Channel_Speed_CodesToJSON(const Fibre_Channel_Speed_Codes& value) {
        nlohmann::ordered_json j;

        j["1200 MBytes/sec compliant (Bit 7)"] = value._1200_MBytes_sec_bit_7;
        j["800 MBytes/sec compliant (Bit 6)"] = value._800_MBytes_sec_bit_6;
        j["1600 MBytes/sec compliant (Bit 5)"] = value._1600_MBytes_sec_bit_5;
        j["400 MBytes/sec compliant (Bit 4)"] = value._400_MBytes_sec_bit_4;
        j["3200 MBytes/sec compliant (Bit 3)"] = value._3200_MBytes_sec_bit_3;
        j["200 MBytes/sec compliant (Bit 2)"] = value._200_MBytes_sec_bit_2;
        j["See byte 62 \"Fibre Channel Speed 2\" (Bit 1)"] = value.see_byte_62_fibre_channel_speed_2_bit_1;
        j["100 MBytes/sec compliant (Bit 0)"] = value._100_MBytes_sec_bit_0;


        return j;
    }

    Fibre_Channel_Speed_Codes Fibre_Channel_Speed_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Fibre_Channel_Speed_Codes complianceCodes;

        complianceCodes._1200_MBytes_sec_bit_7 = j.at("1200 MBytes/sec compliant (Bit 7)").template get<bool>();
        complianceCodes._800_MBytes_sec_bit_6 = j.at("800 MBytes/sec compliant (Bit 6)").template get<bool>();
        complianceCodes._1600_MBytes_sec_bit_5 = j.at("1600 MBytes/sec compliant (Bit 5)").template get<bool>();
        complianceCodes._400_MBytes_sec_bit_4 = j.at("400 MBytes/sec compliant (Bit 4)").template get<bool>();
        complianceCodes._3200_MBytes_sec_bit_3 = j.at("3200 MBytes/sec compliant (Bit 3)").template get<bool>();
        complianceCodes._200_MBytes_sec_bit_2 = j.at("200 MBytes/sec compliant (Bit 2)").template get<bool>();
        complianceCodes.see_byte_62_fibre_channel_speed_2_bit_1 = j.at("See byte 62 \"Fibre Channel Speed 2\" (Bit 1)").template get<bool>();
        complianceCodes._100_MBytes_sec_bit_0 = j.at("100 MBytes/sec compliant (Bit 0)").template get<bool>();


        return complianceCodes;
    }
//############


//############
    nlohmann::ordered_json EncodingToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8024::SFF8472TransceiverEncodingAssignedValues.begin(),
            SFF8024::SFF8472TransceiverEncodingAssignedValues.end(),
            [byte_value](const SFF8024::SFF8472TransceiverEncodingAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8024::SFF8472TransceiverEncodingAssignedValues.end()) {
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
                SFF8024::SFF8472TransceiverEncodingAssignedValues.begin(),
                SFF8024::SFF8472TransceiverEncodingAssignedValues.end(),
                [&strValue](const SFF8024::SFF8472TransceiverEncodingAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8024::SFF8472TransceiverEncodingAssignedValues.end()) throw std::invalid_argument("Encoding is not a known string value");

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

        if(byte_value != 0xFF && byte_value != 0x00) {
            j = (unsigned long)(byte_value) * 100ul;
        } else if(byte_value == 0x00) {
            j = "Not Specified";
        } else {
            j = "> 25.4 GBd";
        }

        return j;
    }

    unsigned char NominalSignalingRate100MBaudFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();
            
            if(strValue == "Not Specified") {
                return 0x00;
            } else if(strValue == "> 25.4 GBd") {
                return 0xFF;
            }

            throw std::invalid_argument("Nominal Signaling Rate can only have value Not Specified or > 25.4 GBd if string");
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
    nlohmann::ordered_json RateIdentifierToJSON(unsigned char byte_value) {
        nlohmann::ordered_json j;

        auto it = std::find_if(
            SFF8472::RateIdentifierAssignedValues.begin(),
            SFF8472::RateIdentifierAssignedValues.end(),
            [byte_value](const SFF8472::RateIdentifierAssignedValue& entry) { return entry.byte_value == byte_value; }
        );

        if(it != SFF8472::RateIdentifierAssignedValues.end()) {
            j = it->name;
        } else {
            j = charToJSONByteStruct(byte_value);
        }

        return j;
    }

    unsigned char RateIdentifierFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();

            auto it = std::find_if(
                SFF8472::RateIdentifierAssignedValues.begin(),
                SFF8472::RateIdentifierAssignedValues.end(),
                [&strValue](const SFF8472::RateIdentifierAssignedValue& entry) { return entry.name == strValue; }
            );

            if(it == SFF8472::RateIdentifierAssignedValues.end()) throw std::invalid_argument("Rate Identifier is not a known string value");

            return it->byte_value;
        } else if(j.is_object()) {
            return charFromJSONByteStruct(j);
        } else {
            throw std::invalid_argument("Rate Identifier has wrong type (neither string nor object)");
        }
    }
//############

//############
    nlohmann::ordered_json LinkLengthInfoToJSON(
        bool copperMode,
        unsigned char byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz,
        unsigned char byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz,
        unsigned char byte_16_length_om2_in_10_m,
        unsigned char byte_17_length_om1_in_10_m,
        unsigned char byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m,
        unsigned char byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value
    ) {
        nlohmann::ordered_json j;

        if(copperMode) {
            j["Type"] = "Copper or Direct Attach";

            if(byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 0) {
                j["Copper Cable Attenuation @ 12.9 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 12.9 GHz [dB]"] = (unsigned long)(byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz);
            }

            if(byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 0) {
                j["Copper Cable Attenuation @ 25.78 GHz [dB]"] = "N/A";
            } else {
                j["Copper Cable Attenuation @ 25.78 GHz [dB]"] = (unsigned long)(byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz);
            }

            //Duplicated to preserve order
            if(byte_16_length_om2_in_10_m == 0xFF) {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_16_length_om2_in_10_m == 0) {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = (unsigned long)(byte_16_length_om2_in_10_m) * 10ul;
            }

            if(byte_17_length_om1_in_10_m == 0xFF) {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_17_length_om1_in_10_m == 0) {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = (unsigned long)(byte_17_length_om1_in_10_m) * 10ul;
            }

            if(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0xFF) {
                j["Length (Copper) or Actual Length (DAC) [m]"] = "> 254 m";
            } else if(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0) {
                j["Length (Copper) or Actual Length (DAC) [m]"] = "N/A";
            } else {
                j["Length (Copper) or Actual Length (DAC) [m]"] = (unsigned long)(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m);
            }

            unsigned char multiplierBits = (byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value & 0b11000000) >> 6;

            if(multiplierBits == 0) { j["Length (Copper) or Actual Length (DAC), Additional, Multiplier"] = "0.1"; }
            else if(multiplierBits == 1) { j["Length (Copper) or Actual Length (DAC), Additional, Multiplier"] = "1"; }
            else if(multiplierBits == 2) { j["Length (Copper) or Actual Length (DAC), Additional, Multiplier"] = "10"; }
            else {j["Length (Copper) or Actual Length (DAC), Additional, Multiplier"] = "100";}

            unsigned char baseValue = byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value & 0b00111111;

            j["Length (Copper) or Actual Length (DAC), Additional, Base Value [m]"] = (unsigned long)(baseValue);
        } else {
            j["Type"] = "Fibre";

            if(byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 0xFF) {
                j["Length (SMF) [km]"] = "> 254 km";
            } else if(byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 0) {
                j["Length (SMF) [km]"] = "N/A";
            } else {
                j["Length (SMF) [km]"] = (unsigned long)(byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz);
            }

            if(byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 0xFF) {
                j["Length (SMF) [m] (Divisible by 100)"] = "> 25.4 km";
            } else if(byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 0) {
                j["Length (SMF) [m] (Divisible by 100)"] = "N/A";
            } else {
                j["Length (SMF) [m] (Divisible by 100)"] = (unsigned long)(byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz) * 100ul;
            }

            //Duplicated to preserve order
            if(byte_16_length_om2_in_10_m == 0xFF) {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_16_length_om2_in_10_m == 0) {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM2 50 um) [m] (Divisible by 10)"] = (unsigned long)(byte_16_length_om2_in_10_m) * 10ul;
            }

            if(byte_17_length_om1_in_10_m == 0xFF) {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_17_length_om1_in_10_m == 0) {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM1 62.5 um) [m] (Divisible by 10)"] = (unsigned long)(byte_17_length_om1_in_10_m) * 10ul;
            }

            if(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0xFF) {
                j["Length (OM4 50 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0) {
                j["Length (OM4 50 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM4 50 um) [m] (Divisible by 10)"] = (unsigned long)(byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m) * 10ul;
            }

            if(byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value == 0xFF) {
                j["Length (OM3 50 um) [m] (Divisible by 10)"] = "> 2.54 km";
            } else if(byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value == 0) {
                j["Length (OM3 50 um) [m] (Divisible by 10)"] = "N/A";
            } else {
                j["Length (OM3 50 um) [m] (Divisible by 10)"] = (unsigned long)(byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value) * 10ul;
            }
        }


        return j;
    }

    struct LinkLengthInfoFromJSONReturn {
        unsigned char byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz;
        unsigned char byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz;
        unsigned char byte_16_length_om2_in_10_m;
        unsigned char byte_17_length_om1_in_10_m;
        unsigned char byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m;
        unsigned char byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value;
    };

    LinkLengthInfoFromJSONReturn LinkLengthInfoFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Link Length must be an object");

        LinkLengthInfoFromJSONReturn parsedStruct;
        parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = 123;

        auto typeStr = j.at("Type").template get<std::string>();

        if(typeStr == "Copper or Direct Attach") {
            const auto& copper_cable_attenuation_12ghz_val = j.at("Copper Cable Attenuation @ 12.9 GHz [dB]");
            if(copper_cable_attenuation_12ghz_val.is_string()) {
                if(copper_cable_attenuation_12ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = 0;
            } else if(copper_cable_attenuation_12ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_12ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] must not be greater than 255");

                parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 12.9 GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& copper_cable_attenuation_25ghz_val = j.at("Copper Cable Attenuation @ 25.78 GHz [dB]");
            if(copper_cable_attenuation_25ghz_val.is_string()) {
                if(copper_cable_attenuation_25ghz_val.template get<std::string>() != "N/A") {
                    throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] has string value, must be N/A");
                }
                parsedStruct.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = 0;
            } else if(copper_cable_attenuation_25ghz_val.is_number_unsigned()) {
                auto numberValue = copper_cable_attenuation_25ghz_val.template get<std::uint64_t>();

                if(numberValue > 255) throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] must not be greater than 255");

                parsedStruct.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = numberValue;
            } else {
                throw std::invalid_argument("Copper Cable Attenuation @ 25.78GHz [dB] has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om2_val = j.at("Length (OM2 50 um) [m] (Divisible by 10)");
            if(length_om2_val.is_string()) {
                if(length_om2_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_16_length_om2_in_10_m = 0;
                } else if(length_om2_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_16_length_om2_in_10_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om2_val.is_number_unsigned()) {
                auto numberValue = length_om2_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_16_length_om2_in_10_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om1_val = j.at("Length (OM1 62.5 um) [m] (Divisible by 10)");
            if(length_om1_val.is_string()) {
                if(length_om1_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_17_length_om1_in_10_m = 0;
                } else if(length_om1_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_17_length_om1_in_10_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om1_val.is_number_unsigned()) {
                auto numberValue = length_om1_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_17_length_om1_in_10_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }


            const auto& length_copper_val = j.at("Length (Copper) or Actual Length (DAC) [m]");
            if(length_copper_val.is_string()) {
                if(length_copper_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = 0;
                } else if(length_copper_val.template get<std::string>() == "> 254 m") {
                    parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (Copper) or Actual Length (DAC) [m] has string value, must be N/A or > 254 m");
                }
            } else if(length_copper_val.is_number_unsigned()) {
                auto numberValue = length_copper_val.template get<std::uint64_t>();

                if(numberValue > 254) throw std::invalid_argument("Length (Copper) or Actual Length (DAC) [m] must not be greater than 254");

                parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (Copper) or Actual Length (DAC) [m] has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_additional_multiplier_val = j.at("Length (Copper) or Actual Length (DAC), Additional, Multiplier");
            if(!length_additional_multiplier_val.is_string()) throw std::invalid_argument("Length (Copper) or Actual Length (DAC), Additional, Multiplier must be a string!");

            unsigned char multiplierBits;
            if(length_additional_multiplier_val.template get<std::string>() == "0.1") { multiplierBits = 0b00; }
            else if(length_additional_multiplier_val.template get<std::string>() == "1") { multiplierBits = 0b01; }
            else if(length_additional_multiplier_val.template get<std::string>() == "10") { multiplierBits = 0b10; }
            else if(length_additional_multiplier_val.template get<std::string>() == "100") { multiplierBits = 0b11; }
            else {
                throw std::invalid_argument("Length (Copper) or Actual Length (DAC), Additional, Multiplier must be either 0.1, 1, 10 or 100");
            }

            const auto& length_additional_base_val = j.at("Length (Copper) or Actual Length (DAC), Additional, Base Value [m]");
            if(!length_additional_base_val.is_number_unsigned()) throw std::invalid_argument("Length (Copper) or Actual Length (DAC), Additional, Base Value [m] must be an unsigned integer!");

            auto length_additional_base_number_value = length_additional_base_val.template get<std::uint64_t>();
            if(length_additional_base_number_value > 63) throw std::invalid_argument("Length (Copper) or Actual Length (DAC), Additional, Base Value [m] must not be greater than 63");

            parsedStruct.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value = (multiplierBits << 6) | (unsigned char)(length_additional_base_number_value);

        } else if(typeStr == "Fibre") {

            const auto& length_smf_km_val = j.at("Length (SMF) [km]");
            if(length_smf_km_val.is_string()) {
                if(length_smf_km_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = 0;
                } else if(length_smf_km_val.template get<std::string>() == "> 254 km") {
                    parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = 0xFF;
                } else {
                    throw std::invalid_argument("Length (SMF) [km] has string value, must be N/A or > 254 km");
                }
            } else if(length_smf_km_val.is_number_unsigned()) {
                auto numberValue = length_smf_km_val.template get<std::uint64_t>();

                if(numberValue > 254) throw std::invalid_argument("Length (SMF) [km] must not be greater than 254");

                parsedStruct.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = numberValue;
            } else {
                throw std::invalid_argument("Length (SMF) [km] has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_smf_m_val = j.at("Length (SMF) [m] (Divisible by 100)");
            if(length_smf_m_val.is_string()) {
                if(length_smf_m_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = 0;
                } else if(length_smf_m_val.template get<std::string>() == "> 25.4 km") {
                    parsedStruct.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = 0xFF;
                } else {
                    throw std::invalid_argument("Length (SMF) [m] (Divisible by 100) has string value, must be N/A or > 25.4 km");
                }
            } else if(length_smf_m_val.is_number_unsigned()) {
                auto numberValue = length_smf_m_val.template get<std::uint64_t>();

                if(numberValue % 100 != 0) throw std::invalid_argument("Length (SMF) [m] (Divisible by 100) must be divisible by 10");
                if(numberValue > 25400) throw std::invalid_argument("Length (SMF) [m] (Divisible by 100) must not be greater than 25400");

                parsedStruct.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = numberValue / 100;
            } else {
                throw std::invalid_argument("Length (SMF) [m] (Divisible by 100) has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om2_val = j.at("Length (OM2 50 um) [m] (Divisible by 10)");
            if(length_om2_val.is_string()) {
                if(length_om2_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_16_length_om2_in_10_m = 0;
                } else if(length_om2_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_16_length_om2_in_10_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om2_val.is_number_unsigned()) {
                auto numberValue = length_om2_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_16_length_om2_in_10_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM2 50 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om1_val = j.at("Length (OM1 62.5 um) [m] (Divisible by 10)");
            if(length_om1_val.is_string()) {
                if(length_om1_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_17_length_om1_in_10_m = 0;
                } else if(length_om1_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_17_length_om1_in_10_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om1_val.is_number_unsigned()) {
                auto numberValue = length_om1_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_17_length_om1_in_10_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM1 62.5 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om4_val = j.at("Length (OM4 50 um) [m] (Divisible by 10)");
            if(length_om4_val.is_string()) {
                if(length_om4_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = 0;
                } else if(length_om4_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om4_val.is_number_unsigned()) {
                auto numberValue = length_om4_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM4 50 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }

            const auto& length_om3_val = j.at("Length (OM3 50 um) [m] (Divisible by 10)");
            if(length_om3_val.is_string()) {
                if(length_om3_val.template get<std::string>() == "N/A") {
                    parsedStruct.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value = 0;
                } else if(length_om3_val.template get<std::string>() == "> 2.54 km") {
                    parsedStruct.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value = 0xFF;
                } else {
                    throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 10) has string value, must be N/A or > 2.54 km");
                }
            } else if(length_om3_val.is_number_unsigned()) {
                auto numberValue = length_om3_val.template get<std::uint64_t>();

                if(numberValue % 10 != 0) throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 10) must be divisible by 10");
                if(numberValue > 2540) throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 10) must not be greater than 2540");

                parsedStruct.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value = numberValue / 10;
            } else {
                throw std::invalid_argument("Length (OM3 50 um) [m] (Divisible by 10) has wrong type (neither string nor unsigned integer)");
            }
        } else {
            throw std::invalid_argument("Link Length has invalid Type string, must be 'Copper or Direct Attach' or Fibre");
        }

        return parsedStruct;
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
    nlohmann::ordered_json VendorRevToJSON(const std::array<unsigned char, 4>& vendorRevRaw) {
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

    std::array<unsigned char, 4> VendorRevFromJSON(const nlohmann::json& j) {
        std::array<unsigned char, 4> arr;


        if(j.is_string()) {
            auto strVal = j.template get<std::string>();
            //We don't care whether it contains unprintable characters - when we serialise and check, it's only to prevent invalid JSON being generated
            //However, as this is a parsed value, we know that the JSON must be valid. Let the user enter what they want...
            if(strVal.size() > 4) throw std::invalid_argument("Vendor Rev must not have more than 4 characters");

            if(strVal.size() < 4) strVal.resize(4, 0x20);

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
            if(decoded.size() != 4) throw std::invalid_argument("Vendor Rev specified as base64 must have exactly 2 characters!");

            std::memcpy(arr.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Rev has invalid type, must be either string or object");
        }

        return arr;
    }
//############

//############
    nlohmann::ordered_json WavelengthOrCableSpecificationComplianceToJSON(
        bool byte_8_sfp_plus_cable_technology_codes_Passive_Cable_bit_2,
        bool byte_8_sfp_plus_cable_technology_codes_Active_Cable_bit_3,
        unsigned char byte_60_wavelength_high_order_byte_or_cable_specification_compliance,
        unsigned char byte_61_wavelength_low_order_byte_or_cable_specification_compliance
    ) {

        nlohmann::ordered_json j;

        if(false && !byte_8_sfp_plus_cable_technology_codes_Passive_Cable_bit_2 && !byte_8_sfp_plus_cable_technology_codes_Active_Cable_bit_3) {
            j["Type"] = "Wavelength";

            static_assert(sizeof(unsigned int) >= 2);
            unsigned int wavelengthRaw = 0;
            wavelengthRaw |= (unsigned(byte_60_wavelength_high_order_byte_or_cable_specification_compliance) << 8);
            wavelengthRaw |= (unsigned(byte_61_wavelength_low_order_byte_or_cable_specification_compliance) << 0);

            j["Wavelength [nm]"] = (unsigned long)(wavelengthRaw);

        } else if(byte_8_sfp_plus_cable_technology_codes_Passive_Cable_bit_2 && !byte_8_sfp_plus_cable_technology_codes_Active_Cable_bit_3) {
            //Passive cable
            j["Type"] = "Passive Cable";

            j["Reserved (Byte 60, Bit 7)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7));
            j["Reserved (Byte 60, Bit 6)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6));
            j["Reserved for SFF-8461 (Byte 60, Bit 5)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5));
            j["Reserved for SFF-8461 (Byte 60, Bit 4)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4));
            j["Reserved for SFF-8461 (Byte 60, Bit 3)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 3));
            j["Reserved for SFF-8461 (Byte 60, Bit 2)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 2));
            j["FC-PI-4 Appendix H compliant (Byte 60, Bit 1)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 1));
            j["SFF-8431 Appendix E compliant (Byte 60, Bit 0)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 0));

            j["Reserved (Byte 61, Bit 7)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7));
            j["Reserved (Byte 61, Bit 6)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6));
            j["Reserved (Byte 61, Bit 5)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5));
            j["Reserved (Byte 61, Bit 4)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4));
            j["Reserved (Byte 61, Bit 3)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3));
            j["Reserved (Byte 61, Bit 2)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2));
            j["Reserved (Byte 61, Bit 1)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1));
            j["Reserved (Byte 61, Bit 0)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0));
        } else {
            //Active cable is also fallback
            j["Type"] = "Active Cable";

            j["Reserved (Byte 60, Bit 7)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7));
            j["Reserved (Byte 60, Bit 6)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6));
            j["Reserved (Byte 60, Bit 5)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5));
            j["Reserved (Byte 60, Bit 4)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4));
            j["FC-PI-4 Limiting compliant (Byte 60, Bit 3)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 3));
            j["SFF-8431 Limiting compliant (Byte 60, Bit 2)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 2));
            j["FC-PI-4 Appendix H compliant (Byte 60, Bit 1)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 1));
            j["SFF-8431 Appendix E compliant (Byte 60, Bit 0)"] = bool(byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 0));

            j["Reserved (Byte 61, Bit 7)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7));
            j["Reserved (Byte 61, Bit 6)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6));
            j["Reserved (Byte 61, Bit 5)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5));
            j["Reserved (Byte 61, Bit 4)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4));
            j["Reserved (Byte 61, Bit 3)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3));
            j["Reserved (Byte 61, Bit 2)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2));
            j["Reserved (Byte 61, Bit 1)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1));
            j["Reserved (Byte 61, Bit 0)"] = bool(byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0));
        }

        return j;
    }

    struct WavelengthOrCableSpecificationComplianceJSONReturn {
        unsigned char byte_60_wavelength_high_order_byte_or_cable_specification_compliance;
        unsigned char byte_61_wavelength_low_order_byte_or_cable_specification_compliance;
    };

    WavelengthOrCableSpecificationComplianceJSONReturn WavelengthOrCableSpecificationComplianceFromJSON(const nlohmann::json& j) {

        if(!j.is_object()) throw std::invalid_argument("Wavelength Or Cable Specification Compliance must be an object");

        WavelengthOrCableSpecificationComplianceJSONReturn parsedStruct;

        auto typeStr = j.at("Type").template get<std::string>();

        if(typeStr == "Wavelength") {

            const auto& wavelengthVal = j.at("Wavelength [nm]");
            if(!wavelengthVal.is_number_unsigned()) throw std::invalid_argument("Wavelength [nm] must be an unsigned number");

            auto wavelengthRaw = wavelengthVal.template get<std::uint64_t>();
            if(wavelengthRaw > 65535) throw std::invalid_argument("Wavelength must not be greater than 65535!");

            parsedStruct.byte_60_wavelength_high_order_byte_or_cable_specification_compliance = (wavelengthRaw >> 8) & (0b11111111);
            parsedStruct.byte_61_wavelength_low_order_byte_or_cable_specification_compliance = (wavelengthRaw >> 0) & (0b11111111);

        } else if(typeStr == "Passive Cable") {

            parsedStruct.byte_60_wavelength_high_order_byte_or_cable_specification_compliance =
                (unsigned char)(j.at("Reserved (Byte 60, Bit 7)").template get<bool>()) << 7 |
                (unsigned char)(j.at("Reserved (Byte 60, Bit 6)").template get<bool>()) << 6 |
                (unsigned char)(j.at("Reserved for SFF-8461 (Byte 60, Bit 5)").template get<bool>()) << 5 |
                (unsigned char)(j.at("Reserved for SFF-8461 (Byte 60, Bit 4)").template get<bool>()) << 4 |
                (unsigned char)(j.at("Reserved for SFF-8461 (Byte 60, Bit 3)").template get<bool>()) << 3 |
                (unsigned char)(j.at("Reserved for SFF-8461 (Byte 60, Bit 2)").template get<bool>()) << 2 |
                (unsigned char)(j.at("FC-PI-4 Appendix H compliant (Byte 60, Bit 1)").template get<bool>()) << 1 |
                (unsigned char)(j.at("SFF-8431 Appendix E compliant (Byte 60, Bit 0)").template get<bool>()) << 0;

            parsedStruct.byte_61_wavelength_low_order_byte_or_cable_specification_compliance =
                (unsigned char)(j.at("Reserved (Byte 61, Bit 7)").template get<bool>()) << 7 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 6)").template get<bool>()) << 6 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 5)").template get<bool>()) << 5 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 4)").template get<bool>()) << 4 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 3)").template get<bool>()) << 3 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 2)").template get<bool>()) << 2 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 1)").template get<bool>()) << 1 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 0)").template get<bool>()) << 0;

        } else if(typeStr == "Active Cable") {
            parsedStruct.byte_60_wavelength_high_order_byte_or_cable_specification_compliance =
                (unsigned char)(j.at("Reserved (Byte 60, Bit 7)").template get<bool>()) << 7 |
                (unsigned char)(j.at("Reserved (Byte 60, Bit 6)").template get<bool>()) << 6 |
                (unsigned char)(j.at("Reserved (Byte 60, Bit 5)").template get<bool>()) << 5 |
                (unsigned char)(j.at("Reserved (Byte 60, Bit 4)").template get<bool>()) << 4 |
                (unsigned char)(j.at("FC-PI-4 Limiting compliant (Byte 60, Bit 3)").template get<bool>()) << 3 |
                (unsigned char)(j.at("SFF-8431 Limiting compliant (Byte 60, Bit 2)").template get<bool>()) << 2 |
                (unsigned char)(j.at("FC-PI-4 Appendix H compliant (Byte 60, Bit 1)").template get<bool>()) << 1 |
                (unsigned char)(j.at("SFF-8431 Appendix E compliant (Byte 60, Bit 0)").template get<bool>()) << 0;

            parsedStruct.byte_61_wavelength_low_order_byte_or_cable_specification_compliance =
                (unsigned char)(j.at("Reserved (Byte 61, Bit 7)").template get<bool>()) << 7 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 6)").template get<bool>()) << 6 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 5)").template get<bool>()) << 5 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 4)").template get<bool>()) << 4 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 3)").template get<bool>()) << 3 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 2)").template get<bool>()) << 2 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 1)").template get<bool>()) << 1 |
                (unsigned char)(j.at("Reserved (Byte 61, Bit 0)").template get<bool>()) << 0;
        } else {
            throw std::invalid_argument("Wavelength Or Cable Specification Compliance has invalid Type string, must be Wavelength, Passive Cable or Active Cable");
        }

        return parsedStruct;
    }
//############

//############
    nlohmann::ordered_json Fibre_Channel_Speed_2_CodesToJSON(const Fibre_Channel_Speed_2_Codes& value) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Reserved (Bit 6)"] = value.reserved_bit_6;
        j["Reserved (Bit 5)"] = value.reserved_bit_5;
        j["Reserved (Bit 4)"] = value.reserved_bit_4;
        j["Reserved (Bit 3)"] = value.reserved_bit_3;
        j["Reserved (Bit 2)"] = value.reserved_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["64 GFC compliant (Bit 0)"] = value._64_GFC_bit_0;

        return j;
    }

    Fibre_Channel_Speed_2_Codes Fibre_Channel_Speed_2_CodesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Fibre_Channel_Speed_2_Codes complianceCodes;

        complianceCodes.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        complianceCodes.reserved_bit_6 = j.at("Reserved (Bit 6)").template get<bool>();
        complianceCodes.reserved_bit_5 = j.at("Reserved (Bit 5)").template get<bool>();
        complianceCodes.reserved_bit_4 = j.at("Reserved (Bit 4)").template get<bool>();
        complianceCodes.reserved_bit_3 = j.at("Reserved (Bit 3)").template get<bool>();
        complianceCodes.reserved_bit_2 = j.at("Reserved (Bit 2)").template get<bool>();
        complianceCodes.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        complianceCodes._64_GFC_bit_0 = j.at("64 GFC compliant (Bit 0)").template get<bool>();

        return complianceCodes;
    }
//############

//############
    nlohmann::ordered_json CC_BASEChecksumToJSON(unsigned char checkSumInProgramming, unsigned char correctChecksum) {
        nlohmann::ordered_json j;

        if(checkSumInProgramming == correctChecksum) {
            j = "auto";
        } else {
            j = charToJSONByteStruct(checkSumInProgramming);
        }

        return j;
    }
    
    //nullopt -> checksum auto in file
    std::optional<unsigned char> CC_BASEChecksumFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();
            if(strValue != "auto") throw std::invalid_argument("CC_BASE string value must be auto");

            return std::nullopt;
        } else if(j.is_object()) {
            return std::make_optional(charFromJSONByteStruct(j));
        } else {
            throw std::invalid_argument("CC_BASE must be either string or object");
        }
    }
//############

//############
    nlohmann::ordered_json OptionValuesToJSON(const Option_Values_Byte_64& byte_64_option_values, const Option_Values_Byte_65& byte_65_option_values) {
        nlohmann::ordered_json j;

        j["Reserved (Byte 64, Bit 7)"] = byte_64_option_values.reserved_bit_7;
        j["High Power Level (Level 4) requirement (Byte 64, Bit 6)"] = byte_64_option_values.high_level_power_level_4_bit_6;
        j["High Power Level (Level 3 or Level 4) requirement (Byte 64, Bit 5)"] = byte_64_option_values.high_level_power_level_4_3_bit_5;
        j["Paging implemented (Byte 64, Bit 4)"] = byte_64_option_values.paging_implemented_indicator_bit_4;
        j["Internal retimer or data recovery (CDR) circuit present (Byte 64, Bit 3)"] = byte_64_option_values.retimer_or_cdr_indicator_bit_3;
        j["Cooled Laser (Byte 64, Bit 2)"] = byte_64_option_values.cooled_transceiver_bit_2;
        j["Power Level 2 requirement (Byte 64, Bit 1)"] = byte_64_option_values.power_level_2_bit_1;
        j["Linear Receiver Output implemented (Byte 64, Bit 0)"] = byte_64_option_values.linear_receiver_output_implemented_bit_0;

        j["Receiver decision threshold implemented (Byte 65, Bit 7)"] = byte_65_option_values.receiver_rdt_implemented_bit_7;
        j["Transmitter tunable (Byte 65, Bit 6)"] = byte_65_option_values.receiver_tunable_bit_6;
        j["RATE_SELECT functionality implemented (Byte 65, Bit 5)"] = byte_65_option_values.rate_select_implemented_bit_5;
        j["TX_DISABLE implemented (Byte 65, Bit 4)"] = byte_65_option_values.tx_disable_implemented_bit_4;
        j["TX_FAULT signal implemented (Byte 65, Bit 3)"] = byte_65_option_values.tx_fault_implemented_bit_3;
        j["Loss of Signal with inverted signal implemented (Byte 65, Bit 2)"] = byte_65_option_values.loss_of_signal_inverted_bit_2;
        j["Loss of Signal implemented (Byte 65, Bit 1)"] = byte_65_option_values.loss_of_signal_implemented_bit_1;
        j["Reserved (Byte 65, Bit 0)"] = byte_65_option_values.reserved_bit_0;


        return j;
    }

    std::tuple<Option_Values_Byte_64, Option_Values_Byte_65> OptionValuesFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Option_Values_Byte_64 byte_64_option_values;
        Option_Values_Byte_65 byte_65_option_values;

        byte_64_option_values.reserved_bit_7 = j.at("Reserved (Byte 64, Bit 7)").template get<bool>();
        byte_64_option_values.high_level_power_level_4_bit_6 = j.at("High Power Level (Level 4) requirement (Byte 64, Bit 6)").template get<bool>();
        byte_64_option_values.high_level_power_level_4_3_bit_5 = j.at("High Power Level (Level 3 or Level 4) requirement (Byte 64, Bit 5)").template get<bool>();
        byte_64_option_values.paging_implemented_indicator_bit_4 = j.at("Paging implemented (Byte 64, Bit 4)").template get<bool>();
        byte_64_option_values.retimer_or_cdr_indicator_bit_3 = j.at("Internal retimer or data recovery (CDR) circuit present (Byte 64, Bit 3)").template get<bool>();
        byte_64_option_values.cooled_transceiver_bit_2 = j.at("Cooled Laser (Byte 64, Bit 2)").template get<bool>();
        byte_64_option_values.power_level_2_bit_1 = j.at("Power Level 2 requirement (Byte 64, Bit 1)").template get<bool>();
        byte_64_option_values.linear_receiver_output_implemented_bit_0 = j.at("Linear Receiver Output implemented (Byte 64, Bit 0)").template get<bool>();

        byte_65_option_values.receiver_rdt_implemented_bit_7 = j.at("Receiver decision threshold implemented (Byte 65, Bit 7)").template get<bool>();
        byte_65_option_values.receiver_tunable_bit_6 = j.at("Transmitter tunable (Byte 65, Bit 6)").template get<bool>();
        byte_65_option_values.rate_select_implemented_bit_5 = j.at("RATE_SELECT functionality implemented (Byte 65, Bit 5)").template get<bool>();
        byte_65_option_values.tx_disable_implemented_bit_4 = j.at("TX_DISABLE implemented (Byte 65, Bit 4)").template get<bool>();
        byte_65_option_values.tx_fault_implemented_bit_3 = j.at("TX_FAULT signal implemented (Byte 65, Bit 3)").template get<bool>();
        byte_65_option_values.loss_of_signal_inverted_bit_2 = j.at("Loss of Signal with inverted signal implemented (Byte 65, Bit 2)").template get<bool>();
        byte_65_option_values.loss_of_signal_implemented_bit_1 = j.at("Loss of Signal implemented (Byte 65, Bit 1)").template get<bool>();
        byte_65_option_values.reserved_bit_0 = j.at("Reserved (Byte 65, Bit 0)").template get<bool>();

        return std::make_tuple(byte_64_option_values, byte_65_option_values);
    }
//############

//############
    nlohmann::ordered_json ExtendedSignalingRateInfoToJSON(
        unsigned char byte_12_nominal_signaling_rate_in_100_mbaud,
        unsigned char byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud,
        unsigned char byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent
    ) {
        nlohmann::ordered_json j;
        if(byte_12_nominal_signaling_rate_in_100_mbaud != 0xFF) {
            j["Type"] = "Standard";

            j["Upper signaling rate margin [%]"] = byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud;

            j["Lower signaling rate margin [%]"] = byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;
        } else {
            j["Type"] = "Extended";

            if(byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud == 0x00) {
                j["Extended Signaling Rate [MBaud] (Divisible by 250)"] = "Unspecified";
            } else {
                j["Extended Signaling Rate [MBaud] (Divisible by 250)"] = (unsigned long)(byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250ul;
            }

            j["Signaling rate margin [%]"] = byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;
        }


        return j;
    }

    struct ExtendedSignalingRateInfoFromJSONReturn {
        unsigned char byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud;
        unsigned char byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;
    };

    ExtendedSignalingRateInfoFromJSONReturn ExtendedSignalingRateFromJSON(const nlohmann::json& j) {

        if(!j.is_object()) throw std::invalid_argument("Extended Signaling Rate must be an object");

        ExtendedSignalingRateInfoFromJSONReturn parsedStruct;

        auto typeStr = j.at("Type").template get<std::string>();

        if(typeStr == "Standard") {
            const auto& upperSignalingRateMarginPercentValue = j.at("Upper signaling rate margin [%]");
            auto upperSignalingRateMarginPercentNumberValue = upperSignalingRateMarginPercentValue.template get<std::uint64_t>();

            if(upperSignalingRateMarginPercentNumberValue > 255) throw std::invalid_argument("Upper signaling rate margin [%] must not be larger than 255");

            parsedStruct.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud = upperSignalingRateMarginPercentNumberValue;
            

            const auto& lowerSignalingRateMarginPercentValue = j.at("Lower signaling rate margin [%]");
            auto lowerSignalingRateMarginPercentNumberValue = upperSignalingRateMarginPercentValue.template get<std::uint64_t>();

            if(lowerSignalingRateMarginPercentNumberValue > 255) throw std::invalid_argument("Lower signaling rate margin [%] must not be larger than 255");

            parsedStruct.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent = lowerSignalingRateMarginPercentNumberValue;
        } else if(typeStr == "Extended") {
            
            const auto& extendedSignalingRateValue = j.at("Extended Signaling Rate [MBaud] (Divisible by 250)");
            if(extendedSignalingRateValue.is_string()) {
                auto strValue = extendedSignalingRateValue.template get<std::string>();
                
                if(strValue != "Unspecified") throw std::invalid_argument("Extended Signaling Rate can only have value Unspecified if string");


                parsedStruct.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud = 0x00;

            } else if(extendedSignalingRateValue.is_number_unsigned()) {
                auto numberValue = extendedSignalingRateValue.template get<std::uint64_t>();

                if(numberValue % 250 != 0) throw std::invalid_argument("Extended Signaling Rate must be divisble by 250");

                if(numberValue > 63750) throw std::invalid_argument("Extended Signaling Rate must not be larger than 63750");

                parsedStruct.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud = numberValue / 250;
            } else {
                throw std::invalid_argument("Extended Signaling Rate has wrong type (neither string nor unsigned integer)");
            }


            const auto& signalingRateMarginPercentValue = j.at("Signaling rate margin [%]");
            auto signalingRateMarginPercentNumberValue = signalingRateMarginPercentValue.template get<std::uint64_t>();

            if(signalingRateMarginPercentNumberValue > 255) throw std::invalid_argument("Signaling rate margin [%] must not be larger than 255");

            parsedStruct.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent = signalingRateMarginPercentNumberValue;

        } else {
            throw std::invalid_argument("Extended Signaling Rate has invalid Type string, must be Standard or Extended");
        }

        return parsedStruct;
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

            j["Year low order digits (Byte 84-85)"] = (unsigned)(digits_of_year);
        } else {
            j["Year low order digits (Byte 84-85)"]["Type"] = "Base64";
            j["Year low order digits (Byte 84-85)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.year_low_order_digits.data(), dateCode.year_low_order_digits.size());
        }
        

        bool dateCodeMonthIsNumber = std::isdigit(dateCode.month_digits[0]) && std::isdigit(dateCode.month_digits[1]);

        if(dateCodeMonthIsNumber) {
            int digits_of_month  = std::stoi(std::string(reinterpret_cast<char const *>(dateCode.month_digits.data()), dateCode.month_digits.size()));

            if(digits_of_month < 1 || digits_of_month > 12) {
                j["Month digits (Byte 86-87)"]["Type"] = "Base64";
                j["Month digits (Byte 86-87)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.month_digits.data(), dateCode.month_digits.size());
            } else {
                j["Month digits (Byte 86-87)"] = (unsigned)(digits_of_month);
            }
        } else {
            j["Month digits (Byte 86-87)"]["Type"] = "Base64";
            j["Month digits (Byte 86-87)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.month_digits.data(), dateCode.month_digits.size());
        }

        bool dateCodeDayIsNumber = std::isdigit(dateCode.day_digits[0]) && std::isdigit(dateCode.day_digits[1]);

        if(dateCodeDayIsNumber) {
            int day  = std::stoi(std::string(reinterpret_cast<char const *>(dateCode.day_digits.data()), dateCode.day_digits.size()));

            if(day < 1 || day > 31) {
                j["Day (Byte 88-89)"]["Type"] = "Base64";
                j["Day (Byte 88-89)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.day_digits.data(), dateCode.day_digits.size());
            } else {
                j["Day (Byte 88-89)"] = (unsigned)(day);
            }
        } else {
            j["Day (Byte 88-89)"]["Type"] = "Base64";
            j["Day (Byte 88-89)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.day_digits.data(), dateCode.day_digits.size());
        }

        bool vendorLotCodePrintable = std::isprint(dateCode.lot_code[0]) && std::isprint(dateCode.lot_code[1]);
        if(vendorLotCodePrintable) {
            
            std::string vendorLotCode = std::string(reinterpret_cast<char const *>(dateCode.lot_code.data()), dateCode.lot_code.size());
            vendorLotCode.erase(std::find_if(vendorLotCode.rbegin(), vendorLotCode.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorLotCode.end());

            j["Vendor Lot Code (Byte 90-91)"] = vendorLotCode;
        } else {
            j["Vendor Lot Code (Byte 90-91)"]["Type"] = "Base64";
            j["Vendor Lot Code (Byte 90-91)"]["Value"] = cppcodec::base64_rfc4648::encode(dateCode.lot_code.data(), dateCode.lot_code.size());
        }
        
        return j;
    }

    DateCode DateCodeFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Date Code must be an object");

        DateCode dateCode;

        auto dateCodeYearVal = j.at("Year low order digits (Byte 84-85)");
        if(dateCodeYearVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeYearVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Year low order digits (Byte 84-85) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.year_low_order_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeYearVal.is_object()) {
            if(dateCodeYearVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Year low order digits (Byte 84-85) object must have Type Base64");

            auto encodedVal = dateCodeYearVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Year low order digits (Byte 84-85) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.year_low_order_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Year low order digits (Byte 84-85) must either be a number or object");
        }

        auto dateCodeMonthVal = j.at("Month digits (Byte 86-87)");
        if(dateCodeMonthVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeMonthVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Month digits (Byte 86-87) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.month_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeMonthVal.is_object()) {
            if(dateCodeMonthVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Month digits (Byte 86-87) object must have Type Base64");

            auto encodedVal = dateCodeMonthVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Month digits (Byte 86-87) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.month_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Month digits (Byte 86-87) must either be a number or object");
        }

        auto dateCodeDayVal = j.at("Day (Byte 88-89)");
        if(dateCodeDayVal.is_number_unsigned()) {
            auto dateCodeNumVal = dateCodeDayVal.template get<std::uint64_t>();
            if(dateCodeNumVal > 99) throw std::invalid_argument("Day (Byte 88-89) must not be greater than 99");

            auto dateCodeStrVal = fmt::format("{:02}", dateCodeNumVal);
            
            //We don't care if the value is not in range... Could as well have been specified as base64
            std::memcpy(dateCode.day_digits.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeDayVal.is_object()) {
            if(dateCodeDayVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Day (Byte 88-89) object must have Type Base64");

            auto encodedVal = dateCodeDayVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Day (Byte 88-89) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.day_digits.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Day (Byte 88-89) must either be a number or object");
        }

        auto dateCodeLotVal = j.at("Vendor Lot Code (Byte 90-91)");
        if(dateCodeLotVal.is_string()) {
            auto dateCodeStrVal = dateCodeLotVal.template get<std::string>();
            if(dateCodeStrVal.size() > 2) throw std::invalid_argument("Vendor Lot Code (Byte 90-91) string must not be longer than 2 characters");
            if(dateCodeStrVal.size() < 2) dateCodeStrVal.resize(2, 0x20);
            
            //We don't care if it's not a valid number... Could as well have been specified as base64
            std::memcpy(dateCode.lot_code.data(), dateCodeStrVal.data(), 2);
        } else if(dateCodeLotVal.is_object()) {
            if(dateCodeLotVal.at("Type").template get<std::string>() != "Base64") throw std::invalid_argument("Vendor Lot Code (Byte 90-91) object must have Type Base64");

            auto encodedVal = dateCodeLotVal.at("Value").template get<std::string>();

            std::vector<uint8_t> decoded;
            try {
                decoded = cppcodec::base64_rfc4648::decode(encodedVal);
            } catch(const cppcodec::parse_error& e) {
                throw std::invalid_argument(fmt::format("Decoding base 64 failed: {}", e.what()));
            }
            if(decoded.size() != 2) throw std::invalid_argument("Vendor Lot Code (Byte 90-91) specified as base64 must have exactly 2 characters!");

            std::memcpy(dateCode.lot_code.data(), decoded.data(), decoded.size());
        } else {
            throw std::invalid_argument("Vendor Lot Code (Byte 90-91) must either be a string or object");
        }

        return dateCode;
    }
//############

//############
    nlohmann::ordered_json Diagnostic_Monitoring_TypeToJSON(const Diagnostic_Monitoring_Type& value) {
        nlohmann::ordered_json j;

        j["Reserved (Bit 7)"] = value.reserved_bit_7;
        j["Digital diagnostic monitoring implemented (Bit 6)"] = value.digital_diagnostic_monitoring_implemented_bit_6;
        j["Internally calibrated (Bit 5)"] = value.internally_calibrated_bit_5;
        j["Externally calibrated (Bit 4)"] = value.externally_calibrated_bit_4;
        j["Received power measurement type (Bit 3)"] = value.received_power_measurement_is_average_bit_3 ? "Average": "OMA";
        j["Address change required (Bit 2)"] = value.address_change_required_bit_2;
        j["Reserved (Bit 1)"] = value.reserved_bit_1;
        j["Reserved (Bit 0)"] = value.reserved_bit_0;

        return j;
    }

    Diagnostic_Monitoring_Type Diagnostic_Monitoring_TypeFromJSON(const nlohmann::json& j) {
        if(!j.is_object()) throw std::invalid_argument("Fibre Channel Technology must be an object");

        Diagnostic_Monitoring_Type value;

        value.reserved_bit_7 = j.at("Reserved (Bit 7)").template get<bool>();
        value.digital_diagnostic_monitoring_implemented_bit_6 = j.at("Digital diagnostic monitoring implemented (Bit 6)").template get<bool>();
        value.internally_calibrated_bit_5 = j.at("Internally calibrated (Bit 5)").template get<bool>();
        value.externally_calibrated_bit_4 = j.at("Externally calibrated (Bit 4)").template get<bool>();

        auto powerMeasurementStr = j.at("Received power measurement type (Bit 3)").template get<std::string>();
        if(powerMeasurementStr == "Average") {
            value.received_power_measurement_is_average_bit_3 = true;
        } else if(powerMeasurementStr == "OMA") {
            value.received_power_measurement_is_average_bit_3 = false;
        } else {
            throw std::invalid_argument("Received power measurement type (Bit 3) string has invalid value, must be either Average or OMA");
        }

        value.address_change_required_bit_2 = j.at("Address change required (Bit 2)").template get<bool>();
        value.reserved_bit_1 = j.at("Reserved (Bit 1)").template get<bool>();
        value.reserved_bit_0 = j.at("Reserved (Bit 0)").template get<bool>();

        return value;
    }
//############

//############
    nlohmann::ordered_json CC_EXTChecksumToJSON(unsigned char checkSumInProgramming, unsigned char correctChecksum) {
        nlohmann::ordered_json j;

        if(checkSumInProgramming == correctChecksum) {
            j = "auto";
        } else {
            j = charToJSONByteStruct(checkSumInProgramming);
        }

        return j;
    }
    
    //nullopt -> checksum auto in file
    std::optional<unsigned char> CC_EXTChecksumFromJSON(const nlohmann::json& j) {
        if(j.is_string()) {
            auto strValue = j.template get<std::string>();
            if(strValue != "auto") throw std::invalid_argument("CC_EXT string value must be auto");

            return std::nullopt;
        } else if(j.is_object()) {
            return std::make_optional(charFromJSONByteStruct(j));
        } else {
            throw std::invalid_argument("CC_EXT must be either string or object");
        }
    }
//############

    void SFF8472_LowerA0hToJSON(nlohmann::ordered_json& j, const SFF8472_LowerA0h& programming, bool copperMode) {

        //Calculate the correct checksums
        //If the checksums in the programming are correct, serialise as Checksum: Auto
        //If the checksums are incorrect, serialise as manual checksum value
        std::vector<unsigned char> binaryBuffer; binaryBuffer.resize(128, 0x00);
        //Use MANUAL_USE_VALUE_IN_PROGRAMMING to avoid double calculation of checksum - we don't want to seek it in the binary
        assembleToBinary(binaryBuffer.data(), programming, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);

        unsigned char correctCC_BASEChecksum = calculateCC_BASEChecksum(binaryBuffer.data());
        unsigned char correctCC_EXTChecksum = calculateCC_EXTChecksum(binaryBuffer.data());


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

        j["Fibre Channel Transmission Media"] = Fibre_Channel_Transmission_Media_CodesToJSON(programming.byte_9_fibre_channel_transmission_media_codes);

        j["Fibre Channel Speed"] = Fibre_Channel_Speed_CodesToJSON(programming.byte_10_fibre_channel_speed_codes);

        j["Encoding"] = EncodingToJSON(programming.byte_11_Encoding);

        j["Nominal Signaling Rate [MBaud] (Divisible by 100)"] = NominalSignalingRate100MBaudToJSON(programming.byte_12_nominal_signaling_rate_in_100_mbaud);

        j["Rate Identifier"] = RateIdentifierToJSON(programming.byte_13_rate_identifier);

        j["Link Length"] = LinkLengthInfoToJSON(
            copperMode,
            programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz,
            programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz,
            programming.byte_16_length_om2_in_10_m,
            programming.byte_17_length_om1_in_10_m,
            programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m,
            programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value
        );

        j["Vendor Name"] = VendorNameToJSON(programming.byte_20_35_vendor_name);

        j["Extended Specification Compliance Codes"] = ExtendedComplianceCodesToJSON(programming.byte_36_extended_specification_compliance_codes);

        j["Vendor OUI"] = VendorOUIToJSON(programming.byte_37_39_vendor_oui);

        j["Vendor Part Number"] = VendorPNToJSON(programming.byte_40_55_vendor_pn);

        j["Vendor Rev"] = VendorRevToJSON(programming.byte_56_59_vendor_rev);

        j["Wavelength Or Cable Specification Compliance"] = WavelengthOrCableSpecificationComplianceToJSON(
            programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2,
            programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3,
            programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance,
            programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance
        );
        

        j["Fibre Channel Speed 2"] = Fibre_Channel_Speed_2_CodesToJSON(programming.byte_62_fibre_channel_2_speed_codes);

        j["CC_BASE"] = CC_BASEChecksumToJSON(programming.byte_63_CC_BASE, correctCC_BASEChecksum);

        j["Option Values"] = OptionValuesToJSON(programming.byte_64_option_values, programming.byte_65_option_values);

        j["Extended Signaling Rate"] = ExtendedSignalingRateInfoToJSON(
            programming.byte_12_nominal_signaling_rate_in_100_mbaud,
            programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud,
            programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent
        );

        j["Vendor Serial Number"] = VendorSNToJSON(programming.byte_68_83_vendor_sn);

        j["Date Code"] = DateCodeToJSON(programming.byte_84_91_date_code);

        j["Diagnostic Monitoring Type"] = Diagnostic_Monitoring_TypeToJSON(programming.byte_92_diagnostic_monitoring_type);

        j["CC_EXT"] = CC_EXTChecksumToJSON(programming.byte_95_CC_EXT, correctCC_EXTChecksum);
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

        programming.byte_9_fibre_channel_transmission_media_codes = Fibre_Channel_Transmission_Media_CodesFromJSON(j.at("Fibre Channel Transmission Media"));

        programming.byte_10_fibre_channel_speed_codes = Fibre_Channel_Speed_CodesFromJSON(j.at("Fibre Channel Speed"));

        programming.byte_11_Encoding = EncodingFromJSON(j.at("Encoding"));

        programming.byte_12_nominal_signaling_rate_in_100_mbaud = NominalSignalingRate100MBaudFromJSON(j.at("Nominal Signaling Rate [MBaud] (Divisible by 100)"));

        programming.byte_13_rate_identifier = RateIdentifierFromJSON(j.at("Rate Identifier"));

        auto linkLengthReturn = LinkLengthInfoFromJSON(j.at("Link Length"));
        programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz = linkLengthReturn.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz;
        programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz = linkLengthReturn.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz;
        programming.byte_16_length_om2_in_10_m = linkLengthReturn.byte_16_length_om2_in_10_m;
        programming.byte_17_length_om1_in_10_m = linkLengthReturn.byte_17_length_om1_in_10_m;
        programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m = linkLengthReturn.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m;
        programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value = linkLengthReturn.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value;

        programming.byte_20_35_vendor_name = VendorNameFromJSON(j.at("Vendor Name"));

        programming.byte_36_extended_specification_compliance_codes = ExtendedComplianceCodesFromJSON(j.at("Extended Specification Compliance Codes"));

        programming.byte_37_39_vendor_oui = VendorOUIFromJSON(j.at("Vendor OUI"));

        programming.byte_40_55_vendor_pn = VendorPNFromJSON(j.at("Vendor Part Number"));

        programming.byte_56_59_vendor_rev = VendorRevFromJSON(j.at("Vendor Rev"));

        auto WavelengthOrCableSpecificationComplianceFromJSONReturn = WavelengthOrCableSpecificationComplianceFromJSON(j.at("Wavelength Or Cable Specification Compliance"));
        programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance = WavelengthOrCableSpecificationComplianceFromJSONReturn.byte_60_wavelength_high_order_byte_or_cable_specification_compliance;
        programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance = WavelengthOrCableSpecificationComplianceFromJSONReturn.byte_61_wavelength_low_order_byte_or_cable_specification_compliance;

        programming.byte_62_fibre_channel_2_speed_codes = Fibre_Channel_Speed_2_CodesFromJSON(j.at("Fibre Channel Speed 2"));

        //CC_BASE is done later

        std::tie(programming.byte_64_option_values, programming.byte_65_option_values) = OptionValuesFromJSON(j.at("Option Values"));

        auto extendedSignalingRateInfoFromJSONReturn = ExtendedSignalingRateFromJSON(j.at("Extended Signaling Rate"));
        programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud = extendedSignalingRateInfoFromJSONReturn.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud;
        programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent = extendedSignalingRateInfoFromJSONReturn.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent;

        programming.byte_68_83_vendor_sn = VendorSNFromJSON(j.at("Vendor Serial Number"));

        programming.byte_84_91_date_code = DateCodeFromJSON(j.at("Date Code"));

        programming.byte_92_diagnostic_monitoring_type = Diagnostic_Monitoring_TypeFromJSON(j.at("Diagnostic Monitoring Type"));

        //CC_EXT is done later


        //Calculate the correct checksums
        std::vector<unsigned char> binaryBuffer; binaryBuffer.resize(256, 0x00);
        //Use MANUAL_USE_VALUE_IN_PROGRAMMING to avoid double calculation of checksum - we don't want to seek it in the binary
        assembleToBinary(binaryBuffer.data(), programming, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);


        auto CC_BASEVal = CC_BASEChecksumFromJSON(j.at("CC_BASE"));
        if(CC_BASEVal.has_value()) {
            programming.byte_63_CC_BASE = CC_BASEVal.value();
        } else {
            //Checksum == Auto
            programming.byte_63_CC_BASE = calculateCC_BASEChecksum(binaryBuffer.data());
        }

        auto CC_EXTVal = CC_EXTChecksumFromJSON(j.at("CC_EXT"));
        if(CC_EXTVal.has_value()) {
            programming.byte_95_CC_EXT = CC_EXTVal.value();
        } else {
            //Checksum == Auto
            programming.byte_95_CC_EXT = calculateCC_EXTChecksum(binaryBuffer.data());
        }
    }   
}