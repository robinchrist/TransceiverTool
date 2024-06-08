#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8636 {
    
    //Expects 256 bytes!
    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h parseBytesToStruct(unsigned char const * bytes);

}