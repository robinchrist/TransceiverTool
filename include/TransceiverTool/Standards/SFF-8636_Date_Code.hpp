#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    //SFF-8636 Rev 2.11 Section 6.3.26 Date Code (00h 212-219)
    struct DateCode {
        std::array<unsigned char, 2> year_low_order_digits;
        std::array<unsigned char, 2> month_digits;
        std::array<unsigned char, 2> day_digits;
        std::array<unsigned char, 2> lot_code;
    };
}