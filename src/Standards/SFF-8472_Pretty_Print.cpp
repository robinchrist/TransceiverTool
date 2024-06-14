#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
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


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Identifier [128]", SFF8472::byteToPhysicalDeviceIdentifierString(programming.byte_0_Identifier));
    str.append("\n");

    return str;

}