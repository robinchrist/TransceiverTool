#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    //SFF-8472 Rev 12.4 Section 6.3.24 Options (00h 193-195)
    struct Option_Values_Byte_64 {
        bool reserved_bit_7;

        // High Power Level Declaration (see SFF-8431 Addendum)
        // Value of zero identifies standard Power Levels 1 ,2 and 3 as indicated by bits 1 and 5.
        // Value of one identifies Power Level 4 requirement. Maximum power is declared in A2h, byte 66.
        bool high_level_power_level_4_bit_6;

        // High Power Level Declaration (see SFF-8431 Addendum)
        // Value of zero identifies standard Power Levels 1 and 2 as indicated by bit 1.
        // Value of one identifies Power Level 3 or Power Level 4 requirement.
        bool high_level_power_level_4_3_bit_5;

        // Paging implemented indicator. A value of 1 indicates that paging is implemented and byte 127d of
        // device address A2h is used for page selection.
        bool paging_implemented_indicator_bit_4;

        // Retimer or CDR indicator. A value of 1 indicates that the transceiver has an internal retimer or clock
        // and data recovery (CDR) circuit.
        bool retimer_or_cdr_indicator_bit_3;

        // Cooled Transceiver Declaration (see SFF-8431).
        // Value of zero identifies a conventional uncooled (or unspecified) laser implementation. Value of one
        // identifies a cooled laser transmitter implementation.
        bool cooled_transceiver_bit_2;

        // Power Level Declaration (see SFF-8431).
        // Value of zero identifies Power Level 1 (or unspecified) requirements.
        // Value of one identifies Power Level 2 requirement.
        // See Table 8-7 and Table 10-1 for control, status, timing.
        // See Bit 5 for Power Level 3 declaration.
        // See Bit 6 for Power Level 4 declaration.
        bool power_level_2_bit_1;

        // Linear Receiver Output Implemented (see SFF-8431).
        // Value of zero identifies a conventional limiting, PAM4 or unspecified receiver output.
        // Value of one identifies a linear receiver output.
        bool linear_receiver_output_implemented_bit_0;
    };

    struct Option_Values_Byte_65 {
        //Receiver decision threshold implemented. A value of 1 indicates that RDT is implemented.
        bool receiver_rdt_implemented_bit_7;

        // Tunable transmitter technology. A value of 1 indicates that the transmitter wavelength/frequency is
        // tunable in accordance with SFF-8690.
        bool receiver_tunable_bit_6;

        // RATE_SELECT functionality is implemented
        bool rate_select_implemented_bit_5;

        // TX_DISABLE is implemented and disables the high speed serial output.
        bool tx_disable_implemented_bit_4;

        // TX_FAULT signal implemented. (See SFF-8419)
        bool tx_fault_implemented_bit_3;

        // Loss of Signal implemented, signal inverted from standard definition in SFP MSA (often called "Signal Detect").
        // NOTE: This is not standard SFP/GBIC behavior and should be avoided, since non-interoperable behavior results
        bool loss_of_signal_inverted_bit_2;

        //Loss of Signal implemented, behavior as defined in SFF-8419 (often called "Rx_LOS").
        bool loss_of_signal_implemented_bit_1;

        bool reserved_bit_0;
    };

}