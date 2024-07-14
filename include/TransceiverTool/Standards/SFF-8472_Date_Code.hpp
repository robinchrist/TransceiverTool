#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    //SFF-8472 Rev 12.4 Section 8.7 Date Code [Address A0h, Bytes 84-91]
    struct DateCode {
        std::array<unsigned char, 2> year_low_order_digits;
        std::array<unsigned char, 2> month_digits;
        std::array<unsigned char, 2> day_digits;
        std::array<unsigned char, 2> lot_code;
    };
}