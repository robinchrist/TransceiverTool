#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    struct Diagnostic_Monitoring_Type {
        bool reserved_bit_7;

        bool reserved_bit_6;

        //Temperature monitoring implemented (0b=Not implemented or pre-Rev 2.8, 1b=Implemented)
        bool temperature_monitoring_implemented_bit_5;

        //Supply voltage monitoring implemented (0b=Not implemented or pre-Rev 2.8, 1b=Implemented)
        bool supply_voltage_monitoring_implemented_bit_4;

        //Received power measurements type. 0=OMA, 1=Average Power
        bool received_power_measurement_is_average_bit_3;

        //Transmitter power measurement. 0=Not supported, 1=Supported
        bool transmitter_power_measurement_supported_bit_2;

        bool reserved_bit_1;

        bool reserved_bit_0;
    };
}