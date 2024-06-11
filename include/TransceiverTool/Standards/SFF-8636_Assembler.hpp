#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"

namespace TransceiverTool::Standards::SFF8636 {


    //AUTO_CALCULATE_FROM_CONTENT: Ignore the byte_191_CC_BASE and byte_223_CC_EXT field in SFF8636_Upper00h and calculate the correct checksum
    //MANUAL_USE_VALUE_IN_PROGRAMMING: Use the values from the byte_191_CC_BASE and byte_223_CC_EXT fields in SFF8636_Upper00h as checksum
    enum class ChecksumDirective {
        AUTO_CALCULATE_FROM_CONTENT,
        MANUAL_USE_VALUE_IN_PROGRAMMING
    };


    //Creates the last 128 bytes of page 00h SFF-8636
    //Needs 256 byte of space in target, will write to last 128 bytes
    void assembleToBinary(unsigned char * target, const SFF8636_Upper00h& programming, ChecksumDirective CC_BASEDirective, ChecksumDirective CC_EXTDirective);
}