#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8024 {

    enum class SFF8636TransceiverEncoding {
        Unspecified,
        _8B_10B,
        _4B_5B,
        NRZ,
        SONET_Scrambled,
        _64B_66B,
        Manchester,
        _256B_257B_transcoded_FEC_enabled_data,
        PAM4,
    };

    struct SFF8636TransceiverEncodingAssignedValue {
        SFF8636TransceiverEncoding enum_value;
        std::string name;
        unsigned char value;
    };

    //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
    inline const std::array<SFF8636TransceiverEncodingAssignedValue, 9> SFF8636TransceiverEncodingAssignedValues {{
        {SFF8636TransceiverEncoding::Unspecified, "Unspecified", 0x00},
        {SFF8636TransceiverEncoding::_8B_10B, "8B/10B", 0x01},
        {SFF8636TransceiverEncoding::_4B_5B, "4B/5B", 0x02},
        {SFF8636TransceiverEncoding::NRZ, "NRZ", 0x03},
        {SFF8636TransceiverEncoding::SONET_Scrambled, "SONET Scrambled", 0x04},
        {SFF8636TransceiverEncoding::_64B_66B, "64B/66B", 0x05},
        {SFF8636TransceiverEncoding::Manchester, "Manchester", 0x06},
        {SFF8636TransceiverEncoding::_256B_257B_transcoded_FEC_enabled_data, "256B/257B (transcoded FEC-enabled data)", 0x07},
        {SFF8636TransceiverEncoding::PAM4, "PAM4", 0x08},
    }};
}