#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    //SFF-8472 Rev 12.4 Section 8.10 Enhanced Options [Address A0h, Byte 93]
    struct Enhanced_Options {
        bool optional_alarm_warning_flags_implemented_bit_7;
        bool optional_soft_TX_DISABLE_control_and_monitoring_implemented_bit_6;
        bool optional_soft_TX_FAULT_monitoring_implemented_bit_5;
        bool optional_soft_RX_LOS_monitoring_implemented_bit_4;
        bool optional_soft_RATE_SELECT_control_and_monitoring_implemented_bit_3;
        bool optional_application_select_control_implemented_per_SFF_8079_bit_2;
        bool optional_soft_rate_select_control_implemented_per_SFF_8431_bit_1;
        bool reserved_bit_0;
    };



}