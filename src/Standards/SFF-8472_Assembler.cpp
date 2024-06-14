#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include <stdexcept>

namespace TransceiverTool::Standards::SFF8472 {


    //Creates the first 128 bytes of page A0h SFF-8472
    void assembleToBinary(unsigned char * target, const SFF8472_LowerA0h& programming, Standards::common::ChecksumDirective CC_BASEDirective, Standards::common::ChecksumDirective CC_EXTDirective) {


    }
}