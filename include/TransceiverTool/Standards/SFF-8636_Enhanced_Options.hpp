#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    struct Enhanced_Options {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool reserved_bit_5;

        //Initialization Complete Flag implemented. This flag was introduced in rev 2.5. When this bit is 1, the initialization
        // complete flag at Byte 6 bit 0 is implemented independently of t_init. When this bit is 0, the initialization complete flag is
        // either not implemented or if implemented has a response time less than t_init, max as specified for the module
        bool initialization_complete_flag_implemented_bit_4;

        //Rate Selection Declaration: When this Declaration bit is 0 the free side device does not support rate selection. When this
        //Declaration bit is 1, rate selection is implemented using extended rate selection. See 6.2.7.2
        bool rate_selection_is_implemented_using_extended_rate_selection_bit_3;

        //This bit is reserved and reads 0. It was used for SFF-8079 support in revisions of this document before 2.10
        bool reserved_bit_2;

        //TC readiness flag implemented. 0= TC readiness flag not implemented. 1= TC readiness flag is implemented.
        bool readiness_flag_implemented_bit_1;

        //Software reset is implemented. Use byte 93, bit 7. 0b = not implemented.
        bool software_reset_implemented_bit_0;
    };
}