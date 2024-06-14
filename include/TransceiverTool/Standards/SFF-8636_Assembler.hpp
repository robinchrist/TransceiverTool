#pragma once
#include <string>
#include <array>
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/common.hpp"

namespace TransceiverTool::Standards::SFF8636 {

    //Creates the last 128 bytes of page 00h SFF-8636
    //Needs 256 byte of space in target, will write to last 128 bytes
    void assembleToBinary(unsigned char * target, const SFF8636_Upper00h& programming, Standards::common::ChecksumDirective CC_BASEDirective, Standards::common::ChecksumDirective CC_EXTDirective);
}