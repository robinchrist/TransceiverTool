#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"

namespace TransceiverTool::Standards::SFF8472 {

    //Expects 128 bytes
    unsigned char calculateCC_BASEChecksum(unsigned char const * bytes) {
        // SFF-8472 Rev 12.4 Section 8.2 CC_BASE [Address A0h, Byte 63]
        // The check code is a one-byte code that can be used to verify that the first 64 bytes of 2-wire interface information
        // in the SFP is valid. The check code shall be the low order 8 bits of the sum of the contents of all the bytes from
        // byte 0 to byte 62, inclusive.

        static_assert(sizeof(unsigned int) >= 2);
        unsigned int sum = 0;
        
        for(int index = 0; index <= 62; ++index) {
            sum += bytes[index];
        }

        return sum & 0b11111111;
    }

    //Expects 128 bytes
    unsigned char calculateCC_EXTChecksum(unsigned char const * bytes) {
        // SFF-8472 Rev 12.4 Section 8.12 CC_EXT [Address A0h, Byte 95]
        // The check code is a one-byte code that can be used to verify that the first 32 bytes of extended 2-wire interface
        // information in the SFP is valid. The check code shall be the low order 8 bits of the sum of the contents of all the
        // bytes from byte 64 to byte 94, inclusive.

        static_assert(sizeof(unsigned int) >= 2);
        unsigned int sum = 0;

        for(int index = 64; index <= 94; ++index) {
            sum += bytes[index];
        }

        return sum & 0b11111111;
    }
}