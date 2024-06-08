#include "TransceiverTool/Standards/SFF-8636_Pretty_Print.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include <iterator>


std::string TransceiverTool::Standards::SFF8636::prettyPrintProgramming(const SFF8636_Upper00h &programming, bool fiberMode, bool copperMode) {

    std::string optionTitleFormatString = "{: <74s}: {:s}\n";
    auto formatReservedBit = [](bool bit) -> std::string {
        if(bit) {
            return fmt::format("{}", fmt::styled("Reserved bit set", fmt::bg(fmt::color::red)));
        } else {
            return "N/A";
        }
    };


    std::string str;

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Type", "SFF-8636");
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Identifier [128]", SFF8024::byteToTransceiverReferenceString(programming.byte_128_Identifier));
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 7-6]", getSFF8636_Extended_Identifier_Bit_7_6Info(programming.byte_129_extended_identifier.power_class_bit_7_6).description
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 5]", programming.byte_129_extended_identifier.power_class_8_bit_5 ? "Power Class 8 implemented (Max power declared in byte 107)" : "Power Class 8 not implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 4]", programming.byte_129_extended_identifier.clei_code_bit_4 ? "CLEI code present in Page 02h" : "No CLEI code present in Page 02h"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 3]", programming.byte_129_extended_identifier.cdr_in_tx_bit_3 ? "CDR present in Tx" : "No CDR in Tx"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 2]", programming.byte_129_extended_identifier.cdr_in_rx_bit_2 ? "CDR present in Rx" : "No CDR in Rx"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Identifier [129, 1-0]", getSFF8636_Extended_Identifier_Bit_1_0Info(programming.byte_129_extended_identifier.power_class_bit_1_0).description
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Connector Type [130]", SFF8024::byteToTransceiverConnectorTypeString(programming.byte_130_Connector_Type)
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 7]", programming.byte_131_ethernet_compliance_codes.extended_bit_7 ? "Extended" : "Standard"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 6]", programming.byte_131_ethernet_compliance_codes._10GBASE_LRM_bit_6 ? "10GBASE-LRM compliant" : "Not 10GBASE-LRM compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 5]", programming.byte_131_ethernet_compliance_codes._10GBASE_LR_bit_5 ? "10GBASE-LR compliant" : "Not 10GBASE-LR compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 4]", programming.byte_131_ethernet_compliance_codes._10GBASE_SR_bit_4 ? "10GBASE-SR compliant" : "Not 10GBASE-SR compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 3]", programming.byte_131_ethernet_compliance_codes._40GBASE_CR4_bit_3 ? "40GBASE-CR4 compliant" : "Not 40GBASE-CR4 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 2]", programming.byte_131_ethernet_compliance_codes._40GBASE_SR4_bit_2 ? "40GBASE-SR4 compliant" : "Not 40GBASE-SR4 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 1]", programming.byte_131_ethernet_compliance_codes._40GBASE_LR4_bit_1 ? "40GBASE-LR4 compliant" : "Not 40GBASE-LR4 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10/40G/100G Ethernet Compliance Codes [131, 0]", programming.byte_131_ethernet_compliance_codes._40G_Active_Cable_XLPPI_bit_0 ? "40G Active Cable (XLPPI) compliant" : "Not 40G Active Cable (XLPPI) compliant"
    );
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 7]", formatReservedBit(programming.byte_132_sonet_compliance_codes.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 6]", formatReservedBit(programming.byte_132_sonet_compliance_codes.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 5]", formatReservedBit(programming.byte_132_sonet_compliance_codes.reserved_bit_5)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 4]", formatReservedBit(programming.byte_132_sonet_compliance_codes.reserved_bit_4)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 3]", formatReservedBit(programming.byte_132_sonet_compliance_codes.reserved_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 2]", programming.byte_132_sonet_compliance_codes.OC48_long_reach_bit_2 ? "OC 48, long reach compliant" : "Not OC 48, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 1]", programming.byte_132_sonet_compliance_codes.OC48_intermediate_reach_bit_1 ? "OC 48, intermediate reach compliant" : "Not OC 48, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [132, 0]", programming.byte_132_sonet_compliance_codes.OC48_short_reach_bit_0 ? "OC 48, short reach compliant" : "Not OC 48, short reach compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 7]", programming.byte_133_sas_sata_compliance_codes.SAS_24_0_Gbps_bit_7 ? "SAS 24.0 Gbps compliant" : "Not SAS 24.0 Gbps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 6]", programming.byte_133_sas_sata_compliance_codes.SAS_12_0_Gbps_bit_6 ? "SAS 12.0 Gbps compliant" : "Not SAS 12.0 Gbps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 5]", programming.byte_133_sas_sata_compliance_codes.SAS_6_0_Gbps_bit_5 ? "SAS 6.0 Gbps compliant" : "Not SAS 6.0 Gbps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 4]", programming.byte_133_sas_sata_compliance_codes.SAS_3_0_Gbps_bit_4 ? "SAS 3.0 Gbps compliant" : "Not SAS 3.0 Gbps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 3]", formatReservedBit(programming.byte_133_sas_sata_compliance_codes.reserved_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 2]", formatReservedBit(programming.byte_133_sas_sata_compliance_codes.reserved_bit_2)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 1]", formatReservedBit(programming.byte_133_sas_sata_compliance_codes.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SAS/SATA Compliance Codes [133, 0]", formatReservedBit(programming.byte_133_sas_sata_compliance_codes.reserved_bit_0)
    );
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 7]", formatReservedBit(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 6]", formatReservedBit(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 5]", formatReservedBit(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 4]", formatReservedBit(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 3]", programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_T_bit_3 ? "1000BASE-T compliant" : "Not 1000BASE-T compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 2]", programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_CX_bit_2 ? "1000BASE-CX compliant" : "Not 1000BASE-CX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 1]", programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_LX_bit_1 ? "1000BASE-LX compliant" : "Not 1000BASE-LX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Gigabit Ethernet Compliance Codes [134, 0]", programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_SX_bit_0 ? "1000BASE-SX compliant" : "Not 1000BASE-SX compliant"
    );
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [135, 7]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Very_long_distance_V_bit_7 ? "Very long distance (V) compliant" : "Not Very long distance (V) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [135, 6]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Short_distance_S_bit_6 ? "Short distance (S) compliant" : "Not Short distance (S) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [135, 5]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Intermediate_distance_I_bit_5 ? "Intermediate distance (I) compliant" : "Not Intermediate distance (I) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [135, 4]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Long_distance_L_bit_4 ? "Long distance (L) compliant" : "Not Long distance (L) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [135, 3]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Medium_M_bit_3 ? "Medium distance (M) compliant" : "Not Medium distance (M) compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [135, 2]", formatReservedBit(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.reserved_Transmitter_Technology_bit_2)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [135, 1]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Longwave_laser_LC_bit_1 ? "Longwave laser (LC) compliant" : "Not Longwave laser (LC) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [135, 0]", programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Electrical_inter_enclosure_EL_bit_0 ? "Electrical inter-enclosure (EL) compliant" : "Not Electrical inter-enclosure (EL) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 7]", programming.byte_136_fibre_channel_transmitter_technology.Electrical_intra_enclosure_bit_7 ? "Electrical intra-enclosure compliant" : "Not Electrical intra-enclosure compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 6]", programming.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_o_OFC_SN_bit_6 ? "Shortwave laser w/o OFC (SN) compliant" : "Not Shortwave laser w/o OFC (SN) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 5]", programming.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_OFC_SL_bit_5 ? "Shortwave laser w OFC (SL) compliant" : "Not Shortwave laser w OFC (SL) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 4]", programming.byte_136_fibre_channel_transmitter_technology.Longwave_Laser_LL_bit_4 ? "Longwave Laser (LL) compliant" : "Not Longwave Laser (LL) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 3]", formatReservedBit(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3)
    ); 
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 2]", formatReservedBit(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2)
    ); 
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 1]", formatReservedBit(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1)
    ); 
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmitter Technology [136, 0]", formatReservedBit(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0)
    ); 
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 7]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Twin Axial Pair (TW) compliant" : "Not Twin Axial Pair (TW) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 6]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Shielded Twisted Pair (TP) compliant" : "Not Shielded Twisted Pair (TP) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 5]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Miniature Coax (MI) compliant" : "Not Miniature Coax (MI) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 4]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Video Coax (TV) compliant" : "Not Video Coax (TV) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 3]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Multi-mode 62.5 um (M6) compliant" : "Not Multi-mode 62.5 um (M6) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 2]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Multi-mode 50 um (M5) compliant" : "Not Multi-mode 50 um (M5) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 1]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Multi-mode 50 um (OM3) compliant" : "Not Multi-mode 50 um (OM3) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [137, 0]", programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 ? "Single Mode (SM) compliant" : "Not Single Mode (SM) compliant"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 7]", programming.byte_138_fibre_channel_speed._1200_MBps_per_channel_bit_7 ? "1200 MBps (per channel) compliant" : "Not 1200 MBps (per channel) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 6]", programming.byte_138_fibre_channel_speed._800_MBps_bit_6 ? "800 MBps compliant" : "Not 800 MBps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 5]", programming.byte_138_fibre_channel_speed._1600_MBps_per_channel_bit_5 ? "1600 MBps (per channel) compliant" : "Not 1600 MBps (per channel) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 4]", programming.byte_138_fibre_channel_speed._400_MBps_bit_4 ? "400 MBps compliant" : "Not 400 MBps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 3]", programming.byte_138_fibre_channel_speed._3200_MBps_per_channel_bit_3 ? "3200 MBps (per channel) compliant" : "Not 3200 MBps (per channel) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 2]", programming.byte_138_fibre_channel_speed._200_MBps_bit_2 ? "200 MBps compliant" : "Not 200 MBps compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 1]", programming.byte_138_fibre_channel_speed.Extended_bit_1 ? "Extended" : "Standard"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [138, 0]", programming.byte_138_fibre_channel_speed._100_MBps_bit_0 ? "100 MBps compliant" : "Not 100 MBps compliant"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Encoding [139]", SFF8024::byteToTransceiverEncodingString(programming.byte_139_Encoding)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Signaling rate, nominal [140]", programming.byte_140_nominal_signaling_rate_in_100_mbaud != 0xFF ? fmt::format("{} MBd", programming.byte_140_nominal_signaling_rate_in_100_mbaud * 100) : "> 25.4GBd (See Byte 222)"
    );
    str.append("\n");


    return str;

}