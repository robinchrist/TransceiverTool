#include "TransceiverTool/Standards/SFF-8636_Checksum.hpp"

namespace TransceiverTool::Standards::SFF8636 {
    //Expects full page 00h with 256 bytes!
    unsigned char calculateCC_BASEChecksum(unsigned char const * bytes) {
        // SFF-8636 Rev 2.11 Section 6.3.22 CC_BASE (00h 191)
        // The check code is a 1-byte code that can be used to verify that the first 63 bytes of serial information in the free
        // side device are valid. The check code shall be the low order 8 bits of the sum of the contents of all the bytes from
        // 128 to 190, inclusive.
        unsigned int sum = 0;
        for(int index = 128; index <= 190; ++index) {
            sum += bytes[index];
        }

        return sum & 0b11111111;
    }

    //Expects full page 00h with 256 bytes!
    unsigned char calculateCC_EXTChecksum(unsigned char const * bytes) {
        // SFF-8636 Rev 2.11 Section 6.3.29 Check Code Extension (00h 223)
        // The check code is a 1-byte code that can be used to verify that the first 32 bytes of extended serial information in
        // the free side device is valid. The check code shall be the low order 8 bits of the sum of the contents of all the bytes
        // from 192 to 222, inclusive.
        unsigned int sum = 0;
        for(int index = 192; index <= 222; ++index) {
            sum += bytes[index];
        }

        return sum & 0b11111111;
    }
}