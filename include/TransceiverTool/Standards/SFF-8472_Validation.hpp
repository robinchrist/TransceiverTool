#pragma once
#include <string>
#include <array>
#include <vector>
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace TransceiverTool::Standards::SFF8472::Validation {

    //SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    void validateIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    //SFF-8472 Rev 12.4 Table 5-2 Physical Device Extended Identifier Values
    void validateExtendedIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    //SFF-8024 Rev 4.11 Table 4-3 Connector Types
    void validateConnectorTypes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateSONETComplianceCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateSFPPlusCableTechnologyCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    //SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateFibreChannelTransmissionMediaCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    common::ValidationResult validateSFF8472_LowerA0h(const SFF8472_LowerA0h& programming);
}