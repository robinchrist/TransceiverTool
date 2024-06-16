#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
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

    return str;

}