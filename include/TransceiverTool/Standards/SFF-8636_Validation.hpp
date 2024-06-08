#pragma once
#include <string>
#include <array>
#include <vector>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636::Validation {

    struct ValidationResult {
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
    };

    //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
    void validateIdentifierValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult);
    
    //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129)
    void validateExtendedIdentifierValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult);
    
    //SFF-8024 Rev 4.11 Table 4-3 Connector Types
    void validateConnectorTypes(const SFF8636_Upper00h& programming, ValidationResult& validationResult);
    
    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    void validateSpecificationComplianceCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult);
    
    //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
    void validateEncodingValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.6 Nominal Signaling Rate (00h 140) & Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    void validateNominalSignalingRate(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
    void validateExtendedRateSelectCompliance(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.14 Vendor Name (00h 148-163)
    void validateVendorName(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Table 6-21 Extended Module Code Values (Page 00h Byte 164)
    void validateExtendedModuleCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.17 Vendor Part Number (00h 168-183)
    void validateVendorPartNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.18 Vendor Revision Number (00h 184-185)
    void validateVendorRevisionNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.21 Maximum Case Temperature (00h 190)
    void validateMaximumCaseTemperature(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8024 Rev 4.11 Table 4-4 Extended Specification Compliance Codes
    void validateExtendedSpecificationComplianceCodes(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Table 6-22 Option Values (Page 00h Bytes 193-195)
    void validateOptionValues(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.25 Vendor Serial Number (00h 196-211)
    void validateVendorSerialNumber(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Date Code (00h 212-219)
    void validateDateCode(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.27 Diagnostic Monitoring Type (00h 220)
    void validateDiagnosticMonitoringType(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Section 6.3.28 Enhanced Options (00h 221)
    void validateEnhancedOptions(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    //SFF-8636 Rev 2.11 Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    void validateExtendedBaudRate(const SFF8636_Upper00h& programming, ValidationResult& validationResult);

    ValidationResult validateSFF8636_Upper00h(const SFF8636_Upper00h& programming);
}