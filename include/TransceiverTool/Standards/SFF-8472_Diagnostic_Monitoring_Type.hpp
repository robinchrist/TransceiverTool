#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    //SFF-8472 Rev 12.4 Section 8.9 Addressing Modes
    struct Diagnostic_Monitoring_Type {
        //Reserved for legacy diagnostic implementations. Must be '0' for compliance with this document.
        bool reserved_bit_7;

        //Digital diagnostic monitoring implemented (described in this document).
        bool digital_diagnostic_monitoring_implemented_bit_6;

        //Internally calibrated
        bool internally_calibrated_bit_5;

        //Externally calibrated
        bool externally_calibrated_bit_4;

        //Received power measurements type. 0=OMA, 1=Average Power
        bool received_power_measurement_is_average_bit_3;

        //Address change required see section above, "addressing modes"
        bool address_change_required_bit_2;

        bool reserved_bit_1;

        bool reserved_bit_0;
    };



}