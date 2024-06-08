#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8024 {

    enum class TransceiverReference {
        Unknown_or_unspecified,
        GBIC,
        Module_connector_soldered_to_motherboard_using_SFF_8472,
        SFP_SFP_plus_SFP28_and_later_with_SFF_8472_management_interface,
        _300_pin_XBI,
        XENPAK,
        XFP,
        XFF,
        XFP_E,
        XPAK,
        X2,
        DWDM_SFP_SFP_plus_not_using_SFF_8472,
        QSFP_INF_8438,
        QSFP_plus_or_later_with_SFF_8636_or_SFF_8436_management_interface_SFF_8436_SFF_8635_SFF_8665_SFF_8685_et_al,
        CXP_or_later,
        Shielded_Mini_Multilane_HD_4X,
        Shielded_Mini_Multilane_HD_8X,
        QSFP28_or_later_with_SFF_8636_management_interface_SFF_8665_et_al,
        CXP2_aka_CXP28_or_later,
        CDFP_Style_1_Style_2_INF_TA_1003,
        Shielded_Mini_Multilane_HD_4X_Fanout_Cable,
        Shielded_Mini_Multilane_HD_8X_Fanout_Cable,
        CDFP_Style_3_INF_TA_1003,
        microQSFP,
        QSFP_DD_Double_Density_8X_Pluggable_Transceiver,
        OSFP_8X_Pluggable_Transceiver,
        SFP_DD_Double_Density_2X_Pluggable_Transceiver_with_SFP_DD_Management_Interface_Specification,
        DSFP_Dual_Small_Form_Factor_Pluggable_Transceiver,
        x4_MiniLink_OcuLink,
        x8_MiniLink,
        QSFP_plus_or_later_with_Common_Management_Interface_Specification_CMIS,
        SFP_DD_Double_Density_2X_Pluggable_Transceiver_with_Common_Management_Interface_Specification_CMIS,
        SFP_plus_and_later_with_Common_Management_Interface_Specification_CMIS,
        OSFP_XD_with_Common_Management_interface_Specification_CMIS,
        OIF_ELSFP_with_Common_Management_interface_Specification_CMIS,
        CDFP_x4_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS,
        CDFP_x8_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS,
        CDFP_x16_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS
    };

    struct TransceiverReferenceAssignedValue {
        TransceiverReference enum_value;
        std::string name;
        unsigned char byte_value;
    };

    //SFF-8024 Rev 4.11 Table 4-1 Identifier Values
    inline const std::array<TransceiverReferenceAssignedValue, 38> TransceiverReferenceAssignedValues {{
        {TransceiverReference::Unknown_or_unspecified, "Unknown or unspecified", 0x00},
        {TransceiverReference::GBIC, "GBIC", 0x01},
        {TransceiverReference::Module_connector_soldered_to_motherboard_using_SFF_8472, "Module/connector soldered to motherboard (using SFF-8472)", 0x02},
        {TransceiverReference::SFP_SFP_plus_SFP28_and_later_with_SFF_8472_management_interface, "SFP/SFP+/SFP28 and later with SFF-8472 management interface", 0x03},
        {TransceiverReference::_300_pin_XBI, "300 pin XBI", 0x04},
        {TransceiverReference::XENPAK, "XENPAK", 0x05},
        {TransceiverReference::XFP, "XFP", 0x06},
        {TransceiverReference::XFF, "XFF", 0x07},
        {TransceiverReference::XFP_E, "XFP-E", 0x08},
        {TransceiverReference::XPAK, "XPAK", 0x09},
        {TransceiverReference::X2, "X2", 0x0A},
        {TransceiverReference::DWDM_SFP_SFP_plus_not_using_SFF_8472, "DWDM-SFP/SFP+ (not using SFF-8472)", 0x0B},
        {TransceiverReference::QSFP_INF_8438, "QSFP (INF-8438)", 0x0C},
        {TransceiverReference::QSFP_plus_or_later_with_SFF_8636_or_SFF_8436_management_interface_SFF_8436_SFF_8635_SFF_8665_SFF_8685_et_al, "QSFP+ or later with SFF-8636 or SFF-8436 management interface (SFF-8436, SFF-8635, SFF-8665, SFF-8685 et al.)", 0x0D},
        {TransceiverReference::CXP_or_later, "CXP or later", 0x0E},
        {TransceiverReference::Shielded_Mini_Multilane_HD_4X, "Shielded Mini Multilane HD 4X", 0x0F},
        {TransceiverReference::Shielded_Mini_Multilane_HD_8X, "Shielded Mini Multilane HD 8X", 0x10},
        {TransceiverReference::QSFP28_or_later_with_SFF_8636_management_interface_SFF_8665_et_al, "QSFP28 or later with SFF-8636 management interface (SFF-8665 et al.)", 0x11},
        {TransceiverReference::CXP2_aka_CXP28_or_later, "CXP2 (aka CXP28) or later", 0x12},
        {TransceiverReference::CDFP_Style_1_Style_2_INF_TA_1003, "CDFP (Style 1/Style 2) INF-TA-1003", 0x13},
        {TransceiverReference::Shielded_Mini_Multilane_HD_4X_Fanout_Cable, "Shielded Mini Multilane HD 4X Fanout Cable", 0x14},
        {TransceiverReference::Shielded_Mini_Multilane_HD_8X_Fanout_Cable, "Shielded Mini Multilane HD 8X Fanout Cable", 0x15},
        {TransceiverReference::CDFP_Style_3_INF_TA_1003, "CDFP (Style 3) INF-TA-1003", 0x16},
        {TransceiverReference::microQSFP, "microQSFP", 0x17},
        {TransceiverReference::QSFP_DD_Double_Density_8X_Pluggable_Transceiver, "QSFP-DD Double Density 8X Pluggable Transceiver", 0x18},
        {TransceiverReference::OSFP_8X_Pluggable_Transceiver, "OSFP 8X Pluggable Transceiver", 0x19},
        {TransceiverReference::SFP_DD_Double_Density_2X_Pluggable_Transceiver_with_SFP_DD_Management_Interface_Specification, "SFP-DD Double Density 2X Pluggable Transceiver with SFP-DD Management Interface Specification", 0x1A},
        {TransceiverReference::DSFP_Dual_Small_Form_Factor_Pluggable_Transceiver, "DSFP Dual Small Form Factor Pluggable Transceiver", 0x1B},
        {TransceiverReference::x4_MiniLink_OcuLink, "x4 MiniLink/OcuLink", 0x1C},
        {TransceiverReference::x8_MiniLink, "x8 MiniLink", 0x1D},
        {TransceiverReference::QSFP_plus_or_later_with_Common_Management_Interface_Specification_CMIS, "QSFP+ or later with Common Management Interface Specification (CMIS)", 0x1E},
        {TransceiverReference::SFP_DD_Double_Density_2X_Pluggable_Transceiver_with_Common_Management_Interface_Specification_CMIS, "SFP-DD Double Density 2X Pluggable Transceiver with Common Management Interface Specification (CMIS)", 0x1F},
        {TransceiverReference::SFP_plus_and_later_with_Common_Management_Interface_Specification_CMIS, "SFP+ and later with Common Management Interface Specification (CMIS)", 0x20},
        {TransceiverReference::OSFP_XD_with_Common_Management_interface_Specification_CMIS, "OSFP-XD with Common Management interface Specification (CMIS)", 0x21},
        {TransceiverReference::OIF_ELSFP_with_Common_Management_interface_Specification_CMIS, "OIF-ELSFP with Common Management interface Specification (CMIS)", 0x22},
        {TransceiverReference::CDFP_x4_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS, "CDFP (x4 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)", 0x23},
        {TransceiverReference::CDFP_x8_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS, "CDFP (x8 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)", 0x24},
        {TransceiverReference::CDFP_x16_PCIe_SFF_TA_1032_with_Common_Management_interface_Specification_CMIS, "CDFP (x16 PCIe) SFF-TA-1032 with Common Management interface Specification (CMIS)", 0x25},
    }};

}