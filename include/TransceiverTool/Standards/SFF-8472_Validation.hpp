// SPDX-FileCopyrightText: 2024-2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <string>
#include <array>
#include <vector>
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace TransceiverTool::Standards::SFF8472::Validation {

    // SFF-8472 Rev 12.4 Table 5-1 Physical Device Identifier Values
    void validateIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Table 5-2 Physical Device Extended Identifier Values
    void
    validateExtendedIdentifierValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8024 Rev 4.11 Table 4-3 Connector Types
    void validateConnectorTypes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateSONETComplianceCodes(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateSFPPlusCableTechnologyCodes(
        const SFF8472_LowerA0h& programming,
        common::ValidationResult& validationResult
    );

    // SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateFibreChannelTransmissionMediaCodes(
        const SFF8472_LowerA0h& programming,
        common::ValidationResult& validationResult
    );

    // SFF-8024 Rev 4.11 Table 4-2 Encoding Values
    void validateEncodingValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Section 5.6 Signaling rate, nominal [Address A0h, Byte 12] & Section 8.4 Signaling Rate,
    // max [Address A0h, Byte 66]
    void validateSignalingRate(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Table 5-6 Rate Identifier
    void validateRateIdentifier(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateVendorName(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
    void validateExtendedSpecificationComplianceCodes(
        const SFF8472_LowerA0h& programming,
        common::ValidationResult& validationResult
    );

    // SFF-8472 Rev 12.4 Section 7.3 Vendor PN [Address A0h, Bytes 40-55]
    void validateVendorPartNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Section 7.4 Vendor Rev [Address A0h, Bytes 56-59
    void validateVendorRevisionNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateWavelengthOrCableSpecificationCompliance(
        const SFF8472_LowerA0h& programming,
        common::ValidationResult& validationResult
    );

    // SFF-8472 Rev 12.4 Table 5-3 Transceiver Compliance Codes
    void validateFibreChannelSpeed2ComplianceCodes(
        const SFF8472_LowerA0h& programming,
        common::ValidationResult& validationResult
    );

    void validateCC_BASEChecksum(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateOptionValues(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Section 8.6 Vendor SN [Address A0h, Bytes 68-83]
    void validateVendorSerialNumber(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    // SFF-8472 Rev 12.4 Section 8.7 Date Code [Address A0h, Bytes 84-91]
    void validateDateCode(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void
    validateDiagnosticMonitoringType(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateEnhancedOptions(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateSFF8472Compliance(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    void validateCC_EXTChecksum(const SFF8472_LowerA0h& programming, common::ValidationResult& validationResult);

    common::ValidationResult validateSFF8472_LowerA0h(const SFF8472_LowerA0h& programming);
}  // namespace TransceiverTool::Standards::SFF8472::Validation
