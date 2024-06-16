#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include <algorithm>
#include <iterator>
#include "TransceiverTool/Vendor_OUIs.hpp"


std::string TransceiverTool::Standards::SFF8472::prettyPrintProgramming(const SFF8472_LowerA0h &programming, bool fiberMode, bool copperMode) {

    std::string optionTitleFormatString = "{: <85s}: {:s}\n";
    auto formatReservedBit = [](bool bit) -> std::string {
        if(bit) {
            return fmt::format("{}", fmt::styled("Reserved bit set", fmt::bg(fmt::color::red)));
        } else {
            return "N/A";
        }
    };


    std::string str;

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Type", "SFF-8472");
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Identifier [0]", SFF8472::byteToPhysicalDeviceIdentifierString(programming.byte_0_Identifier));
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Extended Identifier [1]", SFF8472::byteToPhysicalDeviceExtendedIdentifierString(programming.byte_1_extended_identifier));
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Connector Type [2]", SFF8024::byteToTransceiverConnectorTypeString(programming.byte_2_Connector_type)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 7]", programming.byte_3_ethernet_compliance_codes._10GBASE_ER_bit_7 ? "10GBASE-ER compliant" : "Not 10GBASE-ER compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 6]", programming.byte_3_ethernet_compliance_codes._10GBASE_LRM_bit_6 ? "10GBASE-LRM compliant" : "Not 10GBASE-LRM compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 5]", programming.byte_3_ethernet_compliance_codes._10GBASE_LR_bit_5 ? "10GBASE-LR compliant" : "Not 10GBASE-LR compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 4]", programming.byte_3_ethernet_compliance_codes._10GBASE_SR_bit_4 ? "10GBASE-SR compliant" : "Not 10GBASE-SR compliant"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 3]", programming.byte_3_infiniband_compliance_codes._1X_SX_bit_3 ? "1X SX compliant" : "Not 1X SX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 2]", programming.byte_3_infiniband_compliance_codes._1X_LX_bit_2 ? "1X LX compliant" : "Not 1X LX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 1]", programming.byte_3_infiniband_compliance_codes._1X_Copper_Active_bit_1 ? "1X Copper Active compliant" : "Not 1X Copper Active compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 0]", programming.byte_3_infiniband_compliance_codes._1X_Copper_Passive_bit_0 ? "1X Copper Passive compliant" : "Not 1X Copper Passive compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, ESCON Compliance Codes [4, 7]", programming.byte_4_escon_compliance_codes.ESCON_MMF_1310nm_LED_bit_7 ? "ESCON MMF, 1310nm LED compliant" : "Not ESCON MMF, 1310nm LED compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, ESCON Compliance Codes [4, 6]", programming.byte_4_escon_compliance_codes.ESCON_SMF_1310nm_Laser_bit_6 ? "ESCON SMF, 1310nm Laser compliant" : "Not ESCON SMF, 1310nm Laser compliant"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 5]", programming.byte_4_5_sonet_compliance_codes.OC_192_short_reach_byte_4_bit_5 ? "OC-192, short reach compliant" : "Not OC-192, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 4-3]", 
        fmt::format(
            "{} ({:#04b})", 
            getSFF8472_SONETReachSpecifierInfo(programming.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3).name,
            getSFF8472_SONETReachSpecifierInfo(programming.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3).byte_value
        )
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 2]", programming.byte_4_5_sonet_compliance_codes.OC_48_long_reach_byte_4_bit_2 ? "OC-48, long reach compliant" : "Not OC-48, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 1]", programming.byte_4_5_sonet_compliance_codes.OC_48_intermediate_reach_byte_4_bit_1 ? "OC-48, intermediate reach compliant" : "Not OC-48, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 0]", programming.byte_4_5_sonet_compliance_codes.OC_48_short_reach_byte_4_bit_0 ? "OC-48, short reach compliant" : "Not OC-48, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 7]", formatReservedBit(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 6]", programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_long_reach_2_byte_5_bit_6 ? "OC-12, single mode, long reach compliant" : "Not OC-12, single mode, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 5]", programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_intermediate_reach_byte_5_bit_5 ? "OC-12, single mode, intermediate reach compliant" : "Not OC-12, single mode, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 4]", programming.byte_4_5_sonet_compliance_codes.OC_12_short_reach_byte_5_bit_4 ? "OC-12, short reach compliant" : "Not OC-12, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 3]", formatReservedBit(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 2]", programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_long_reach_byte_5_bit_2 ? "OC-3, single mode, long reach compliant" : "Not OC-3, single mode, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 1]", programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_intermediate_reach_byte_5_bit_1 ? "OC-3, single mode, intermediate reach compliant" : "Not OC-3, single mode, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 0]", programming.byte_4_5_sonet_compliance_codes.OC_3_short_reach__byte_5_bit_0 ? "OC-3, short reach compliant" : "Not OC-3, short reach compliant"
    );

    str.append("\n");

    return str;

}