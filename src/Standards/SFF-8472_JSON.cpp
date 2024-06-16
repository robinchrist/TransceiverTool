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
    }   
}