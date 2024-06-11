#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    //SFF-8636 Rev 2.11 Section 6.3.24 Options (00h 193-195)
    struct Option_Values_Byte_193 {
        bool reserved_bit_7;

        //LPMode/TxDis input signal is configurable using byte 99, bit 1. See SFF-8679 for a complete description.
        bool lpmode_txdis_input_configurable_bit_6;

        //IntL/RxLOSL output signal is configurable using byte 99, bit 0. See SFF-8679 for a complete description
        bool intl_rxlosl_output_configurable_bit_5;

        //Tx input adaptive equalizers freeze capable. 1 if implemented, else 0.
        bool tx_input_adaptive_equalizers_freeze_capable_bit_4;

        //Tx input equalizers auto-adaptive capable. 1 if implemented, else 0.
        bool tx_input_equalizers_auto_adaptive_capable_bit_3;

        //Tx input equalizers fixed-programmable settings. 1 if implemented, else 0.
        bool tx_input_equalizers_fixed_programmable_settings_bit_2;

        //Rx output emphasis fixed-programmable settings. 1 if implemented, else 0.
        bool rx_output_emphasis_fixed_programmable_settings_bit_1;

        //Rx output amplitude fixed-programmable settings. 1 if implemented, else 0.
        bool rx_output_amplitude_fixed_programmable_settings_bit_0;
    };

    struct Option_Values_Byte_194 {
        //Tx CDR On/Off Control implemented. 1 if controllable, else 0
        bool tx_cdr_on_off_control_implemented_bit_7;
        
        //Rx CDR On/Off Control implemented. 1 if controllable, else 0
        bool rx_cdr_on_off_control_implemented_bit_6;

        //Tx CDR Loss of Lock (LOL) flag implemented. 1 if implemented, else 0
        bool tx_cdr_loss_of_lock_lol_flag_implemented_bit_5;
    
        //Rx CDR Loss of Lock (LOL) flag implemented. 1 if implemented, else 0
        bool rx_cdr_loss_of_lock_lol_flag_implemented_bit_4;

        //Rx Squelch Disable implemented. 1 if implemented, else 0
        bool rx_squelch_disable_implemented_bit_3;

        //Rx Output Disable implemented. 1 if implemented, else 0
        bool rx_output_disable_implemented_bit_2;

        //Tx Squelch Disable implemented. 1 if implemented, else 0
        bool tx_squelch_disable_implemented_bit_1;

        //Tx Squelch implemented. 1 if implemented, else 0
        bool tx_squelch_implemented_bit_0;
    };

    struct Option_Values_Byte_195 {
        //Memory Page 02 provided. 1 if implemented, else 0.
        bool memory_page_02_provided_bit_7;

        //Memory Page 01h provided. 1 if implemented, else 0.
        bool memory_page_01h_provided_bit_6;

        //Rate select is implemented as defined in 6.2.7. If the bit is set to 1 then refer to that section for multi-rate operation description.
        bool rate_select_implemented_bit_5;

        //Tx_Disable is implemented and disables the serial output as defined by the relevant transmitter specification.
        bool tx_disable_implemented_bit_4;

        //Tx_Fault signal implemented. 1 if implemented, else 0
        bool tx_fault_signal_implemented_bit_3;

        //Tx Squelch implemented to reduce OMA coded 0, implemented to reduce Pave coded 1.
        bool tx_squelch_implemented_to_reduce_pave_bit_2;

        //Tx Loss of Signal implemented. 1 if implemented, else 0
        bool tx_loss_of_signal_implemented_bit_1;

        //Pages 20-21h implemented. Default = 0 (not implemented)
        bool pages_20_to_21h_implemented_bit_0;
    };
}