#include <cstddef>
#include <variant>
#include <string>
#include <array>
#include <vector>
#include <fmt/core.h>
#include <algorithm>


struct SFF8024TransceiverReferenceAssignedValue {
    unsigned char value;
    std::string name;
};

//SFF-8024 Rev 4.11 Table 4-1 Identifier Values
const std::array<SFF8024TransceiverReferenceAssignedValue, 38> SFF8024TransceiverReferenceAssignedValues {{
    {0x00, "Unknown or unspecified"},
    {0x01, "GBIC"},
    {0x02, "Module/connector soldered to motherboard (using SFF-8472)"},
    {0x03, "SFP/SFP+/SFP28 and later with SFF-8472 management interface"},
    {0x04, "300 pin XBI"},
    {0x05, "XENPAK"},
    {0x06, "XFP"},
    {0x07, "XFF"},
    {0x08, "XFP-E"},
    {0x09, "XPAK"},
    {0x0A, "X2"},
    {0x0B, "DWDM-SFP/SFP+ (not using SFF-8472)"},
    {0x0C, "QSFP (INF-8438)"},
    {0x0D, "QSFP+ or later with SFF-8636 or SFF-8436 management interface (SFF-8436, SFF-8635, SFF-8665, SFF-8685 et al.)"},
    {0x0E, "CXP or later"},
    {0x0F, "Shielded Mini Multilane HD 4X"},
    {0x10, "Shielded Mini Multilane HD 8X"},
    {0x11, "QSFP28 or later with SFF-8636 management interface (SFF-8665 et al.)"},
    {0x12, "CXP2 (aka CXP28) or later"},
    {0x13, "CDFP (Style 1/Style 2) INF-TA-1003"},
    {0x14, "Shielded Mini Multilane HD 4X Fanout Cable"},
    {0x15, "Shielded Mini Multilane HD 8X Fanout Cable"},
    {0x16, "CDFP (Style 3) INF-TA-1003"},
    {0x17, "microQSFP"},
    {0x18, "QSFP-DD Double Density 8X Pluggable Transceiver"},
    {0x19, "OSFP 8X Pluggable Transceiver"},
    {0x1A, "SFP-DD Double Density 2X Pluggable Transceiver with SFP-DD Management Interface Specification"},
    {0x1B, "DSFP Dual Small Form Factor Pluggable Transceiver"},
    {0x1C, "x4 MiniLink/OcuLink"},
    {0x1D, "x8 MiniLink"},
    {0x1E, "QSFP+ or later with Common Management Interface Specification (CMIS)"},
    {0x1F, "SFP-DD Double Density 2X Pluggable Transceiver with Common Management Interface Specification (CMIS)"},
    {0x20, "SFP+ and later with Common Management Interface Specification (CMIS)"},
    {0x21, "OSFP-XD with Common Management interface Specification (CMIS)"},
    {0x22, "OIF-ELSFP with Common Management interface Specification (CMIS)"},
    {0x23, "CDFP (x4 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)"},
    {0x24, "CDFP (x8 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)"},
    {0x25, "CDFP (x16 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)"}
}};

//SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129) bits 7-6
enum class SFF8636_Extended_Identifier_Bit_7_6 {
    POWER_CLASS_1_0b00,
    POWER_CLASS_2_0b01,
    POWER_CLASS_3_0b10,
    POWER_CLASS_4_TO_7_0b11
};
struct SFF8636_Extended_Identifier_Bit_7_6_string {
    SFF8636_Extended_Identifier_Bit_7_6 enum_value;
    std::string description;
    unsigned char bitValue;
};
const std::array<SFF8636_Extended_Identifier_Bit_7_6_string, 4> SFF8636_Extended_Identifier_Bit_7_6_strings {{
    {SFF8636_Extended_Identifier_Bit_7_6::POWER_CLASS_1_0b00, "Power Class 1 (1.5 W max.)", 0b00},
    {SFF8636_Extended_Identifier_Bit_7_6::POWER_CLASS_2_0b01, "Power Class 2 (2.0 W max.)", 0b01},
    {SFF8636_Extended_Identifier_Bit_7_6::POWER_CLASS_3_0b10, "Power Class 3 (2.5 W max.)", 0b10},
    {SFF8636_Extended_Identifier_Bit_7_6::POWER_CLASS_4_TO_7_0b11, "Power Class 4 (3.5 W max.) and Power Classes 5, 6 or 7", 0b11},
}};
const SFF8636_Extended_Identifier_Bit_7_6_string& getSFF8636_Extended_Identifier_Bit_7_6Info(SFF8636_Extended_Identifier_Bit_7_6 enum_value) {
    return *std::find_if(
        SFF8636_Extended_Identifier_Bit_7_6_strings.begin(),
        SFF8636_Extended_Identifier_Bit_7_6_strings.end(),
        [enum_value](const SFF8636_Extended_Identifier_Bit_7_6_string& entry) { return entry.enum_value == enum_value; }
    );
}

//SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129) bits 1-0
enum class SFF8636_Extended_Identifier_Bit_1_0 {
    POWER_CLASSES_1_TO_4_0b00,
    POWER_CLASS_5_0b01,
    POWER_CLASS_6_0b10,
    POWER_CLASS_7_0b11,
};
struct SFF8636_Extended_Identifier_Bit_1_0_string {
    SFF8636_Extended_Identifier_Bit_1_0 enum_value;
    std::string description;
    unsigned char bitValue;
};
const std::array<SFF8636_Extended_Identifier_Bit_1_0_string, 4> SFF8636_Extended_Identifier_Bit_1_0_strings {{
    {SFF8636_Extended_Identifier_Bit_1_0::POWER_CLASSES_1_TO_4_0b00, "Power Classes 1 to 4", 0b00},
    {SFF8636_Extended_Identifier_Bit_1_0::POWER_CLASS_5_0b01, "Power Class 5 (4.0 W max.)", 0b01},
    {SFF8636_Extended_Identifier_Bit_1_0::POWER_CLASS_6_0b10, "Power Class 6 (4.5 W max.)", 0b10},
    {SFF8636_Extended_Identifier_Bit_1_0::POWER_CLASS_7_0b11, "Power Class 7 (5.0 W max.)", 0b11},
}};
const SFF8636_Extended_Identifier_Bit_1_0_string& getSFF8636_Extended_Identifier_Bit_1_0Info(SFF8636_Extended_Identifier_Bit_1_0 enum_value) {
    return *std::find_if(
        SFF8636_Extended_Identifier_Bit_1_0_strings.begin(),
        SFF8636_Extended_Identifier_Bit_1_0_strings.end(),
        [enum_value](const SFF8636_Extended_Identifier_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
    );
}

struct SFF8636_Extended_Identifier {
    //Power Class Related Bit 7 & 6
    SFF8636_Extended_Identifier_Bit_7_6 power_class_bit_7_6;

    //Power Class 8 implemented (Max power declared in byte 107)
    bool power_class_8_bit_5;

    //CLEI code present in Page 02h (0 = Not Present, 1 = Present)
    bool clei_code_bit_4;

    //CDR present in Tx (0 = No CDR in Tx, 1 = CDR present in Tx)
    bool cdr_in_tx_bit_3;

    //CDR present in Tx (0 = No CDR in Rx, 1 = CDR present in Rx)
    bool cdr_in_rx_bit_2;

    //Power Class Related Bit 1 & 0
    SFF8636_Extended_Identifier_Bit_1_0 power_class_bit_1_0;
};


struct SFF8024TransceiverConnectorTypeAssignedValue {
    unsigned char value;
    std::string name;
};

//SFF-8024 Rev 4.11 Table 4-3 Connector Types
const std::array<SFF8024TransceiverConnectorTypeAssignedValue, 38> SFF8024TransceiverConnectorTypeAssignedValues {{
    {0x00, "Unknown or unspecified"},
    {0x01, "SC (Subscriber Connector)"},
    {0x02, "Fibre Channel Style 1 copper connector"},
    {0x03, "Fibre Channel Style 2 copper connector"},
    {0x04, "BNC/TNC (Bayonet/Threaded Neill-Concelman)"},
    {0x05, "Fibre Channel coax headers"},
    {0x06, "Fiber Jack"},
    {0x07, "LC (Lucent Connector)"},
    {0x08, "MT-RJ (Mechanical Transfer - Registered Jack)"},
    {0x09, "MU (Multiple Optical)"},
    {0x0A, "SG"},
    {0x0B, "Optical Pigtail"},
    {0x0C, "MPO 1x12 (Multifiber Parallel Optic)"},
    {0x0D, "MPO 2x16"},
    //Gap 0x0Eh-0x1F: Reserved
    {0x20, "HSSDC II (High Speed Serial Data Connector)"},
    {0x21, "Copper pigtail"},
    {0x22, "RJ45 (Registered Jack)"},
    {0x23, "No separable connector"},
    {0x24, "MXC 2x16"},
    {0x25, "CS optical connector"},
    {0x26, "SN (previously Mini CS) optical connector"},
    {0x27, "MPO 2x12"},
    {0x28, "MPO 1x16"}
}};



//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 131
//10/40G/100G Ethernet Compliance Codes
struct SFF8636_10_40G_100G_Ethernet_Compliance_Codes {
    bool extended_bit_7;
    bool _10GBASE_LRM_bit_6;
    bool _10GBASE_LR_bit_5;
    bool _10GBASE_SR_bit_4;
    bool _40GBASE_CR4_bit_3;
    bool _40GBASE_SR4_bit_2;
    bool _40GBASE_LR4_bit_1;
    bool _40G_Active_Cable_XLPPI_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 132
//SONET Compliance Codes
struct SFF8636_SONET_Compliance_Codes {
    bool reserved_bit_7;
    bool reserved_bit_6;
    bool reserved_bit_5;
    bool reserved_bit_4;
    bool reserved_bit_3;
    bool OC48_long_reach_bit_2;
    bool OC48_intermediate_reach_bit_1;
    bool OC48_short_reach_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 133
//SAS/SATA Compliance Codes
struct SFF8636_SAS_SATA_Compliance_Codes {
    bool SAS_24_0_Gbps_bit_7;
    bool SAS_12_0_Gbps_bit_6;
    bool SAS_6_0_Gbps_bit_5;
    bool SAS_3_0_Gbps_bit_4;
    bool reserved_bit_3;
    bool reserved_bit_2;
    bool reserved_bit_1;
    bool reserved_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 134
//Gigabit Ethernet Compliance Codes
struct SFF8636_Gigabit_Ethernet_Compliance_Codes {
    bool reserved_bit_7;
    bool reserved_bit_6;
    bool reserved_bit_5;
    bool reserved_bit_4;
    bool _1000BASE_T_bit_3;
    bool _1000BASE_CX_bit_2;
    bool _1000BASE_LX_bit_1;
    bool _1000BASE_SX_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 135
//Fibre Channel Link Length & Fibre Channel Transmitter Technology
struct SFF8636_Fibre_Channel_Link_Length_And_Transmitter_Technology {
    bool Very_long_distance_V_bit_7;
    bool Short_distance_S_bit_6;
    bool Intermediate_distance_I_bit_5;
    bool Long_distance_L_bit_4;
    bool Medium_M_bit_3;
    bool reserved_Transmitter_Technology_bit_2;
    bool Longwave_laser_LC_bit_1;
    bool Electrical_inter_enclosure_EL_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 136
//Fibre Channel Transmitter Technology
struct SFF8636_Fibre_Channel_Transmitter_Technology {
    bool Electrical_intra_enclosure_bit_7;
    bool Shortwave_laser_w_o_OFC_SN_bit_6;
    bool Shortwave_laser_w_OFC_SL_bit_5;
    bool Longwave_Laser_LL_bit_4;
    bool reserved_bit_3;
    bool reserved_bit_2;
    bool reserved_bit_1;
    bool reserved_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 137
//Fibre Channel Transmission Media
struct SFF8636_Fibre_Channel_Transmission_Media {
    bool Twin_Axial_Pair_TW_bit_7;
    bool Shielded_Twisted_Pair_TP_bit_6;
    bool Miniature_Coax_MI_bit_5;
    bool Video_Coax_TV_bit_4;
    bool Multi_mode_62_5_um_M6_bit_3;
    bool Multi_mode_50_um_M5_bit_2;
    bool Multi_mode_50_um_OM3_bit_1;
    bool Single_Mode_SM_bit_0;
};

//SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138) byte 137
//Fibre Channel Speed
struct SFF8636_Fibre_Channel_Speed {
    bool _1200_MBps_per_channel_bit_7;
    bool _800_MBps_bit_6;
    bool _1600_MBps_per_channel_bit_5;
    bool _400_MBps_bit_4;
    bool _3200_MBps_per_channel_bit_3;
    bool _200_MBps_bit_2;
    bool Extended_bit_1;
    bool _100_MBps_bit_0;
};



struct SFF8024_SFF8636TransceiverEncodingAssignedValue {
    unsigned char value;
    std::string name;
};

//SFF-8024 Rev 4.11 Table 4-2 Encoding Values
const std::array<SFF8024_SFF8636TransceiverEncodingAssignedValue, 38> SFF8024_SFF8636TransceiverEncodingAssignedValues {{
    {0x00, "Unspecified"},
    {0x01, "8B/10B"},
    {0x02, "4B/5B"},
    {0x03, "NRZ"},
    {0x04, "SONET Scrambled"},
    {0x05, "64B/66B"},
    {0x06, "Manchester"},
    {0x07, "256B/257B (transcoded FEC-enabled data)"},
    {0x08, "PAM4"},
}};


//SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
enum class SFF8636_Extended_Rate_Select_Compliance_Bit_1_0 {
    RESERVED_0b00,
    RATE_SELECT_VERSION_1_0b01,
    RATE_SELECT_VERSION_2_0b10,
    RESERVED_0b11
};
struct SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_string {
    SFF8636_Extended_Rate_Select_Compliance_Bit_1_0 enum_value;
    std::string description;
    unsigned char bitValue;
};
const std::array<SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_string, 4> SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_strings {{
    {SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b00, "Reserved (00b)", 0b00},
    {SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RATE_SELECT_VERSION_1_0b01, "Rate Select Version 1", 0b01},
    {SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RATE_SELECT_VERSION_2_0b10, "Rate Select Version 2", 0b10},
    {SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b11, "Reserved (11b)", 0b11},
}};
const SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_string& getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(SFF8636_Extended_Rate_Select_Compliance_Bit_1_0 enum_value) {
    return *std::find_if(
        SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_strings.begin(),
        SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_strings.end(),
        [enum_value](const SFF8636_Extended_Rate_Select_Compliance_Bit_1_0_string& entry) { return entry.enum_value == enum_value; }
    );
}
struct SFF8636_Extended_Rate_Select_Compliance {
    bool reserved_bit_7;
    bool reserved_bit_6;
    bool reserved_bit_5;
    bool reserved_bit_4;
    bool reserved_bit_3;
    bool reserved_bit_2;
    SFF8636_Extended_Rate_Select_Compliance_Bit_1_0 rate_select_bits_1_0;
};

//SFF-8636 Rev 2.11 Table 6-19 Device Technology (Page 00h Byte 147) & Table 6-20 Transmitter Technology (Page 00h Byte 147 Bits 7-4)
enum class SFF8636_Transmitter_Technology {
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
struct SFF8636_Transmitter_Technology_string {
    SFF8636_Transmitter_Technology enum_value;
    std::string description;
    unsigned char bitValue;
};
const std::array<SFF8636_Transmitter_Technology_string, 16> SFF8636_Transmitter_Technology_strings {{
    {SFF8636_Transmitter_Technology::_850_nm_VCSEL_0b0000, "850 nm VCSEL", 0b0000},
    {SFF8636_Transmitter_Technology::_1310_nm_VCSEL_0b0001, "1310 nm VCSEL", 0b0001},
    {SFF8636_Transmitter_Technology::_1550_nm_VCSEL_0b0010, "1550 nm VCSEL", 0b0010},
    {SFF8636_Transmitter_Technology::_1310_nm_FP_0b0011, "1310 nm FP", 0b0011},
    {SFF8636_Transmitter_Technology::_1310_nm_DFB_0b0100, "1310 nm DFB", 0b0100},
    {SFF8636_Transmitter_Technology::_1550_nm_DFB_0b0101, "1550 nm DFB", 0b0101},
    {SFF8636_Transmitter_Technology::_1310_nm_EML_0b0110, "1310 nm EML", 0b0110},
    {SFF8636_Transmitter_Technology::_1550_nm_EML_0b0111, "1550 nm EML", 0b0111},
    {SFF8636_Transmitter_Technology::Other_Undefined_0b1000, "Other / Undefined", 0b1000},
    {SFF8636_Transmitter_Technology::_1490_nm_DFB_0b1001, "1490 nm DFB", 0b1001},
    {SFF8636_Transmitter_Technology::Copper_cable_unequalized_0b1010, "Copper cable unequalized", 0b1010},
    {SFF8636_Transmitter_Technology::Copper_cable_passive_equalized_0b1011, "Copper cable passive equalized", 0b1011},
    {SFF8636_Transmitter_Technology::Copper_cable_near_and_far_end_limiting_active_equalizers_0b1100, "Copper cable, near and far end limiting active equalizers", 0b1100},
    {SFF8636_Transmitter_Technology::Copper_cable_far_end_limiting_active_equalizers_0b1101, "Copper cable, far end limiting active equalizers", 0b1101},
    {SFF8636_Transmitter_Technology::Copper_cable_near_end_limiting_active_equalizers_0b1110, "Copper cable, near end limiting active equalizers", 0b1110},
    {SFF8636_Transmitter_Technology::Copper_cable_linear_active_equalizers_0b1111, "Copper cable, linear active equalizers", 0b1111}
}};
struct SFF8636_Device_Technology_and_Transmitter_Technology {
    //Device Technology bits 7-4
    SFF8636_Transmitter_Technology transmitter_technology_bit_7_4;

    //0 = No wavelength control, 1 = Active wavelength control
    bool wavelength_control_bit_3;

    //0 = Uncooled transmitter device, 1 = Cooled transmitter
    bool cooled_transmitter_bit_2;

    //0 = Pin detector, 1 = APD detector
    bool pin_apd_detector_bit_1;

    //0 = Transmitter not tunable, 1 = Transmitter tunable
    bool transmitter_tunable_bit_0;
};


struct SFF8636_Upper00h {

    //Byte 128: Identifier
    //Identifier Type of free side device (See SFF-8024 Transceiver Management)
    unsigned char byte_128_Identifier;

    //Byte 129: Extended Identifier (Ext. Identifier in spec)
    //Extended Identifier of free side device. Includes power classes, CLEI codes, CDR capability (See Table 6-16)
    SFF8636_Extended_Identifier byte_129_extended_identifier;


    //Byte 130: Connector Type
    //Code for media connector type (See SFF-8024 Transceiver Management)
    unsigned char byte_130_Connector_Type;

    //Bytes 131-138: Specification Compliance
    //Code for electronic or optical compatibility (See Table 6-17)

    //Byte 131: 10/40G/100G Ethernet Compliance Codes
    SFF8636_10_40G_100G_Ethernet_Compliance_Codes byte_131_ethernet_compliance_codes;
    
    //Byte 132: SONET Compliance Codes
    SFF8636_SONET_Compliance_Codes byte_132_sonet_compliance_codes;

    //Byte 133: SAS/SATA Compliance Codes
    SFF8636_SAS_SATA_Compliance_Codes byte_133_sas_sata_compliance_codes;

    //Byte 134: Gigabit Ethernet Compliance Codes
    SFF8636_Gigabit_Ethernet_Compliance_Codes byte_134_gigabit_ethernet_compliance_codes;

    //Byte 135: Fibre Channel Link Length (7-3) & Fibre Channel Transmitter Technology (2-0)
    SFF8636_Fibre_Channel_Link_Length_And_Transmitter_Technology byte_135_fibre_channel_link_length_and_transmitter_technology;

    //Byte 136: Fibre Channel Transmitter Technology
    SFF8636_Fibre_Channel_Transmitter_Technology byte_136_fibre_channel_transmitter_technology;

    //Byte 137: Fibre Channel Transmission Media
    SFF8636_Fibre_Channel_Transmission_Media byte_137_fibre_channel_transmission_media;

    //Byte 138: Fibre Channel Speed
    SFF8636_Fibre_Channel_Speed byte_138_fibre_channel_speed;

    //Byte 139: Encoding
    //Code for serial encoding algorithm. (See SFF-8024 Transceiver Management)
    unsigned char byte_139_Encoding;

    //Byte 140: Nominal Signaling Rate
    //Nominal signaling rate, units of 100 MBd. For rate > 25.4 GBd, set this to FFh and use Byte 222
    unsigned char byte_140_nominal_signaling_rate_in_100_mbaud;

    //Byte 141: Extended Rate Select Compliance
    //Tags for extended rate select compliance. See Table 6-18.
    SFF8636_Extended_Rate_Select_Compliance byte_141_extended_rate_select_compliance;

    //Byte 141: Length (SMF)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for SMF fiber in km. A value of 1 shall be used for reaches from 0 to 1 km.
    unsigned char byte_142_length_smf_in_kilometers;

    //Byte 142: Length (OM3 50 um)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for EBW 50/125 um fiber (OM3), units of 2 m
    unsigned char byte_143_length_om3_in_2m;

    //Byte 143: Length (OM2 50 um)
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 50/125 um fiber (OM2), units of 1 m
    unsigned char byte_144_length_om4_in_m;

    //Byte 144: Length (OM1 62.5 um) or Copper Cable Attenuation
    //Link length supported at the signaling rate in byte 140 or page 00h byte 222, for 62.5/125 um fiber (OM1), units of 1 m, 
    //or copper cable attenuation in dB at 25.78 GHz.
    unsigned char byte_145_length_om1_in_1m_or_copper_cable_attentuation_in_dB;


    //Byte 145: Length (passive copper or active cable or OM4 50 um)
    //Length of passive or active cable assembly (units of 1 m) or link length supported at the signaling rate in byte 140 or page 00h byte 222,
    //for OM4 50/125 um fiber (units of 2 m) as indicated by Byte 147. See 6.3.12
    unsigned char byte_146_length_copper_in_1m_or_om4_in_2m;

    //Device technology
    //Device technology (Table 6-19 and Table 6-20).
    SFF8636_Device_Technology_and_Transmitter_Technology byte_147_device_technology_and_transmitter_technology;


    //Byte 222: Extended Baud Rate: Nominal
    //Nominal baud rate per channel, units of 250 MBd. Complements Byte 140. See Table 6-26.
    unsigned char byte_222_extended_baud_rate_in_250_mbaud;
};

struct ValidationResult {
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

//TODO: Introduce options to not warn on values in "Vendor Specific" ranges (in case this tool is used by actual vendor?)
ValidationResult validateSFF8636_Upper00h(const SFF8636_Upper00h& programming) {

    std::vector<std::string> warnings;
    std::vector<std::string> errors;

    //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
    if(programming.byte_128_Identifier >= 0x26 && programming.byte_128_Identifier <= 0x7F) {
        errors.push_back(
            fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
        );
    }
    if(programming.byte_128_Identifier >= 0x80) {
        warnings.push_back(
            fmt::format("Byte 128 (\"Identifier\") value corresponds to \"Vendor Specific\" range (SFF-8024 Rev 4.11 Table 4-1 \"Identifier Values\"), value is {:#04x}", programming.byte_128_Identifier)
        );
    }

    //SFF-8636 Rev 2.11 Table 6-16 Extended Identifier Values (Page 00h Byte 129)
    //If bit 1-0 is not 00 (i.e. Power Class 5/6/7), bit 7-6 must indicate 11 (Power Class 4-7)
    if(programming.byte_129_extended_identifier.power_class_bit_1_0 != SFF8636_Extended_Identifier_Bit_1_0::POWER_CLASSES_1_TO_4_0b00 &&
        programming.byte_129_extended_identifier.power_class_bit_7_6 != SFF8636_Extended_Identifier_Bit_7_6::POWER_CLASS_4_TO_7_0b11
    ) {
        errors.push_back(
            //TODO: add bits value present in programming to error message?
            fmt::format("Byte 129 (\"Extended Identifier\") inconsistent values: value bits 1-0 indicate power class \"{}\", but bits 7-6 indicates \"{}\"", 
                getSFF8636_Extended_Identifier_Bit_1_0Info(programming.byte_129_extended_identifier.power_class_bit_1_0).description,
                getSFF8636_Extended_Identifier_Bit_7_6Info(programming.byte_129_extended_identifier.power_class_bit_7_6).description
            )
        );
    }


    //SFF-8024 Rev 4.11 Table 4-3 Connector Types
    if(
        (programming.byte_130_Connector_Type >= 0x0E && programming.byte_130_Connector_Type <= 0x1F) ||
        (programming.byte_130_Connector_Type >= 0x29 && programming.byte_130_Connector_Type <= 0x7F)
    ) {
        errors.push_back(
            fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
        );
    }
    if(programming.byte_130_Connector_Type >= 0x80) {
        warnings.push_back(
            fmt::format("Byte 130 (\"Connector Type\") value corresponds to \"Vendor specific\" range (SFF-8024 Rev 4.11 Table 4-3 \"Connector Types\"), value is {:#04x}")
        );
    }

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    //Byte 132: SONET Compliance Codes
    if(programming.byte_132_sonet_compliance_codes.reserved_bit_7 || programming.byte_132_sonet_compliance_codes.reserved_bit_6 ||
        programming.byte_132_sonet_compliance_codes.reserved_bit_5 || programming.byte_132_sonet_compliance_codes.reserved_bit_4 ||
        programming.byte_132_sonet_compliance_codes.reserved_bit_3
    ) {
        errors.push_back(
            fmt::format(
                "Byte 132 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}",
                programming.byte_132_sonet_compliance_codes.reserved_bit_7, programming.byte_132_sonet_compliance_codes.reserved_bit_6,
                programming.byte_132_sonet_compliance_codes.reserved_bit_5, programming.byte_132_sonet_compliance_codes.reserved_bit_4,
                programming.byte_132_sonet_compliance_codes.reserved_bit_3
            )
        );
    }

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    //Byte 133: SAS/SATA Compliance Codes
    if(programming.byte_133_sas_sata_compliance_codes.reserved_bit_3 || programming.byte_133_sas_sata_compliance_codes.reserved_bit_2 ||
        programming.byte_133_sas_sata_compliance_codes.reserved_bit_1 || programming.byte_133_sas_sata_compliance_codes.reserved_bit_0
    ) {
        errors.push_back(
            fmt::format(
                "Byte 133 (\"SAS/SATA Compliance Codes\") value has at least one reserved bit set: Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                programming.byte_133_sas_sata_compliance_codes.reserved_bit_3, programming.byte_133_sas_sata_compliance_codes.reserved_bit_2, 
                programming.byte_133_sas_sata_compliance_codes.reserved_bit_1, programming.byte_133_sas_sata_compliance_codes.reserved_bit_0
            )
        );
    }

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    //Byte 134: Gigabit Ethernet Compliance Codes
    if(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7 || programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6 ||
        programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5 || programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4
    ) {
        errors.push_back(
            fmt::format(
                "Byte 134 (\"Gigabit Ethernet Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}",
                programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7, programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6,
                programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5, programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4
            )
        );
    }

    //SFF-8636 Rev 2.11 Table 6-17 Specification Compliance Codes (Page 00h Bytes 131-138)
    //Byte 136: Fibre Channel Transmitter Technology
    if(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3 || programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2 ||
        programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1 || programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0
    ) {
        errors.push_back(
            fmt::format(
                "Byte 136 (\"Fibre Channel Transmitter Technology\") value has at least one reserved bit set: Bit 3 {:d}, Bit 2 {:d}, Bit 1 {:d}, Bit 0 {:d}",
                programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2, 
                programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1, programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0
            )
        );
    }

    //SFF-8024 Rev 4.11 Table 4-2 Encoding Values
    if(programming.byte_139_Encoding >= 0x09) {
        errors.push_back(
            fmt::format("Byte 139 (\"Encoding\") value corresponds to \"Reserved\" range (SFF-8024 Rev 4.11 Table 4-2 \"Encoding Values\"), value is {:#04x}", programming.byte_139_Encoding)
        );
    }

    //SFF-8636 Rev 2.11 Section 6.3.6 Nominal Signaling Rate (00h 140) & Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0) {
        warnings.push_back(
            fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0")
        );
    }
    if(programming.byte_140_nominal_signaling_rate_in_100_mbaud == 0xFF) {
        if(programming.byte_222_extended_baud_rate_in_250_mbaud == 0) {
            warnings.push_back(
            fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but Byte 222 (\"Extended Baud Rate: Nominal\") is zero")
            );
        } else {
            unsigned long nominalBaudRateFromByte222 = (unsigned long)(programming.byte_222_extended_baud_rate_in_250_mbaud) * 250UL;

            if(nominalBaudRateFromByte222 < 25400) {
                warnings.push_back(
                fmt::format("Byte 140 (\"Nominal Signaling Rate\") value is 0xFF indicating a nominal baud rate > 25.4 GBd, but value from Byte 222 (\"Extended Baud Rate: Nominal\") is too small ({} MBd)", nominalBaudRateFromByte222)
                );
            }
        }
    }

    //SFF-8636 Rev 2.11 Table 6-18 Extended Rate Select Compliance Tag Assignment (Page 00h Byte 141)
    if(programming.byte_141_extended_rate_select_compliance.reserved_bit_7 || programming.byte_141_extended_rate_select_compliance.reserved_bit_6 ||
        programming.byte_141_extended_rate_select_compliance.reserved_bit_5 || programming.byte_141_extended_rate_select_compliance.reserved_bit_4 ||
        programming.byte_141_extended_rate_select_compliance.reserved_bit_3 || programming.byte_141_extended_rate_select_compliance.reserved_bit_2 ||
        programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b00 ||
        programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 == SFF8636_Extended_Rate_Select_Compliance_Bit_1_0::RESERVED_0b11
    ) {
        errors.push_back(
            fmt::format(
                "Byte 132 (\"SONET Compliance Codes\") value has at least one reserved bit set: Bit 7 {:d}, Bit 6 {:d}, Bit 5 {:d}, Bit 4 {:d}, Bit 3 {:d}, Bit 2 {:d}, Bit 1-0: \"{}\"",
                programming.byte_141_extended_rate_select_compliance.reserved_bit_7, programming.byte_141_extended_rate_select_compliance.reserved_bit_6,
                programming.byte_141_extended_rate_select_compliance.reserved_bit_5, programming.byte_141_extended_rate_select_compliance.reserved_bit_4,
                programming.byte_141_extended_rate_select_compliance.reserved_bit_3, programming.byte_141_extended_rate_select_compliance.reserved_bit_2,
                getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0)
            )
        );
    }

    
    //SFF-8636 Rev 2.11 Table 6-26 Extended Baud Rate: Nominal (Page 00h Byte 222)
    if(programming.byte_222_extended_baud_rate_in_250_mbaud != 0 && programming.byte_140_nominal_signaling_rate_in_100_mbaud != 0xFF) {
        warnings.push_back(
            fmt::format("Byte 222 (\"Extended Baud Rate: Nominal\") is nonzero, but byte 140 (\"Nominal Signaling Rate\") is not 0xFF")
        );
    }

    return ValidationResult {
        std::move(warnings),
        std::move(errors)
    };
}

int main() {
    
    return 0;
}