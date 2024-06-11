#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {

    //Expects full page 00h with 256 bytes!
    unsigned char calculateCC_BASEChecksum(unsigned char const * bytes);

    //Expects full page 00h with 256 bytes!
    unsigned char calculateCC_EXTChecksum(unsigned char const * bytes);
}