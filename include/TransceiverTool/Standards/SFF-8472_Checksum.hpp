#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8472 {

    //Expects 128 bytes
    unsigned char calculateCC_BASEChecksum(unsigned char const * bytes);

    //Expects 128 bytes
    unsigned char calculateCC_EXTChecksum(unsigned char const * bytes);
}