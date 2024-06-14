#pragma once

namespace TransceiverTool::Standards::common {

    //For SFF8636:
    //AUTO_CALCULATE_FROM_CONTENT: Ignore the byte_191_CC_BASE and byte_223_CC_EXT field in SFF8636_Upper00h and calculate the correct checksum
    //MANUAL_USE_VALUE_IN_PROGRAMMING: Use the values from the byte_191_CC_BASE and byte_223_CC_EXT fields in SFF8636_Upper00h as checksum
    //FOR SFF8472:
    //TODO
    enum class ChecksumDirective {
        AUTO_CALCULATE_FROM_CONTENT,
        MANUAL_USE_VALUE_IN_PROGRAMMING
    };
}