#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    //SFF-8636 Rev 2.11 Table 6-21 Extended Module Code Values (Page 00h Byte 164)
    struct Extended_Module_Codes {
        bool reserved_bit_7;
        bool reserved_bit_6;
        bool HDR_bit_5;
        bool EDR_bit_4;
        bool FDR_bit_3;
        bool QDR_bit_2;
        bool DDR_bit_1;
        bool SDR_bit_0;
    };

}