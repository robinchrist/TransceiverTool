#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8636 {
    //SFF-8636 Rev 2.11 Table 6-19 Device Technology (Page 00h Byte 147) & Table 6-20 Transmitter Technology (Page 00h Byte 147 Bits 7-4)
    enum class Transmitter_Technology {
        _850_nm_VCSEL_0b0000,
        _1310_nm_VCSEL_0b0001,
        _1550_nm_VCSEL_0b0010,
        _1310_nm_FP_0b0011,
        _1310_nm_DFB_0b0100,
        _1550_nm_DFB_0b0101,
        _1310_nm_EML_0b0110,
        _1550_nm_EML_0b0111,
        Other_Undefined_0b1000,
        _1490_nm_DFB_0b1001,
        Copper_cable_unequalized_0b1010,
        Copper_cable_passive_equalized_0b1011,
        Copper_cable_near_and_far_end_limiting_active_equalizers_0b1100,
        Copper_cable_far_end_limiting_active_equalizers_0b1101,
        Copper_cable_near_end_limiting_active_equalizers_0b1110,
        Copper_cable_linear_active_equalizers_0b1111
    };
    struct Transmitter_Technology_string {
        Transmitter_Technology enum_value;
        std::string description;
        unsigned char bitValue;
    };
    const std::array<Transmitter_Technology_string, 16> Transmitter_Technology_strings {{
        {Transmitter_Technology::_850_nm_VCSEL_0b0000, "850 nm VCSEL", 0b0000},
        {Transmitter_Technology::_1310_nm_VCSEL_0b0001, "1310 nm VCSEL", 0b0001},
        {Transmitter_Technology::_1550_nm_VCSEL_0b0010, "1550 nm VCSEL", 0b0010},
        {Transmitter_Technology::_1310_nm_FP_0b0011, "1310 nm FP", 0b0011},
        {Transmitter_Technology::_1310_nm_DFB_0b0100, "1310 nm DFB", 0b0100},
        {Transmitter_Technology::_1550_nm_DFB_0b0101, "1550 nm DFB", 0b0101},
        {Transmitter_Technology::_1310_nm_EML_0b0110, "1310 nm EML", 0b0110},
        {Transmitter_Technology::_1550_nm_EML_0b0111, "1550 nm EML", 0b0111},
        {Transmitter_Technology::Other_Undefined_0b1000, "Other / Undefined", 0b1000},
        {Transmitter_Technology::_1490_nm_DFB_0b1001, "1490 nm DFB", 0b1001},
        {Transmitter_Technology::Copper_cable_unequalized_0b1010, "Copper cable unequalized", 0b1010},
        {Transmitter_Technology::Copper_cable_passive_equalized_0b1011, "Copper cable passive equalized", 0b1011},
        {Transmitter_Technology::Copper_cable_near_and_far_end_limiting_active_equalizers_0b1100, "Copper cable, near and far end limiting active equalizers", 0b1100},
        {Transmitter_Technology::Copper_cable_far_end_limiting_active_equalizers_0b1101, "Copper cable, far end limiting active equalizers", 0b1101},
        {Transmitter_Technology::Copper_cable_near_end_limiting_active_equalizers_0b1110, "Copper cable, near end limiting active equalizers", 0b1110},
        {Transmitter_Technology::Copper_cable_linear_active_equalizers_0b1111, "Copper cable, linear active equalizers", 0b1111}
    }};
    struct Device_Technology_and_Transmitter_Technology {
        //Device Technology bits 7-4
        Transmitter_Technology transmitter_technology_bit_7_4;

        //0 = No wavelength control, 1 = Active wavelength control
        bool wavelength_control_bit_3;

        //0 = Uncooled transmitter device, 1 = Cooled transmitter
        bool cooled_transmitter_bit_2;

        //0 = Pin detector, 1 = APD detector
        bool pin_apd_detector_bit_1;

        //0 = Transmitter not tunable, 1 = Transmitter tunable
        bool transmitter_tunable_bit_0;
    };
}