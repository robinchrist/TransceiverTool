// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
#include "TransceiverTool/Standards/SFF-8024_Encoding_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Extended_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Rate_Identifiers.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

template <typename Table>
nlohmann::ordered_json names(const Table& table) {
    auto result = nlohmann::ordered_json::array();
    for(const auto& entry : table) {
        result.push_back(entry.name);
    }
    return result;
}

int main() {
    namespace S = TransceiverTool::Standards;
    nlohmann::ordered_json result;
    auto& a = result["SFF-8472_LowerA0h.json"];
    a["sff-8472_identifier"] = names(S::SFF8472::PhysicalDeviceIdentifierAssignedValues);
    a["sff-8472_extended_identifier"] = names(S::SFF8472::PhysicalDeviceExtendedIdentifierAssignedValues);
    a["sff-8024_connector_type"] = names(S::SFF8024::TransceiverConnectorTypeAssignedValues);
    a["sff-8472_encoding"] = names(S::SFF8024::SFF8472TransceiverEncodingAssignedValues);
    a["sff-8472_rate_identifier"] = names(S::SFF8472::RateIdentifierAssignedValues);
    a["sff-8472_extended_specification_compliance_codes"] = names(S::SFF8024::ExtendedComplianceCodesAssignedValues);
    a["sff-8472_compliance"] = names(S::SFF8472::SFF_8472_ComplianceAssignedValues);
    auto& b = result["SFF-8636_Upper00h.json"];
    b["sff-8636_identifier"] = names(S::SFF8024::TransceiverReferenceAssignedValues);
    b["sff-8636_connector_type"] = names(S::SFF8024::TransceiverConnectorTypeAssignedValues);
    b["sff-8636_encoding"] = names(S::SFF8024::SFF8636TransceiverEncodingAssignedValues);
    b["sff-8636_extended_specification_compliance_codes"] = names(S::SFF8024::ExtendedComplianceCodesAssignedValues);
    std::cout << result.dump(2) << '\n';
}
