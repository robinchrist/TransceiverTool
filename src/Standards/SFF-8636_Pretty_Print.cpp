#include "TransceiverTool/Standards/SFF-8636_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_Checksum.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include <algorithm>
#include <iterator>
#include "TransceiverTool/Vendor_OUIs.hpp"


std::string TransceiverTool::Standards::SFF8636::prettyPrintProgramming(const SFF8636_Upper00h &programming, bool fiberMode, bool copperMode) {

    std::string optionTitleFormatString = "{: <85s}: {:s}\n";
    auto formatReservedBit = [](bool bit) -> std::string {
        if(bit) {
            return fmt::format("{}", fmt::styled("Reserved bit set", fmt::bg(fmt::color::red)));
        } else {
            return "N/A";
        }
    };

    std::vector<unsigned char> binary; binary.resize(256, 0x00);
    assembleToBinary(binary.data(), programming, ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);

    unsigned char CC_BASE = calculateCC_BASEChecksum(binary.data());
    unsigned char CC_EXT = calculateCC_EXTChecksum(binary.data());


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
        "Signaling rate, nominal [140]", programming.byte_140_nominal_signaling_rate_in_100_mbaud != 0xFF ? fmt::format("{} MBd", unsigned(programming.byte_140_nominal_signaling_rate_in_100_mbaud) * 100) : "> 25.4GBd (See Byte 222)"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 7]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 6]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 5]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_5)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 4]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_4)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 3]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 2]", formatReservedBit(programming.byte_141_extended_rate_select_compliance.reserved_bit_2)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Rate Select Compliance [141, 1-0]", getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0).description
    );
    str.append("\n");
    

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Length (SMF) [142]", programming.byte_142_length_smf_in_kilometers > 1 ? fmt::format("{} km", programming.byte_142_length_smf_in_kilometers) : (programming.byte_142_length_smf_in_kilometers == 1 ? "0 - 1 km" : "Not Applicable (e.g. AOC) / SM fiber not supported / must be determined from the free side device")
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Length (OM3 50 um) [143]", programming.byte_143_length_om3_in_2m > 1 ? fmt::format("{} m", programming.byte_143_length_om3_in_2m * 2) : "Not Applicable (e.g. AOC) / OM3 fiber type not supported / must be determined from the free side device"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Length (OM2 50 um) [144]", programming.byte_144_length_om2_in_m > 1 ? fmt::format("{} m", programming.byte_144_length_om2_in_m) : "Not Applicable (e.g. AOC) / OM2 fiber type not supported / must be determined from the free side device"
    );
    str.append("\n");



    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (OM1 62.5 um) [145]", programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB > 1 ? fmt::format("{} m", programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB) : "Not Applicable (e.g. AOC) / OM1 fiber type not supported / must be determined from the free side device"
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 25.78GHz [145]", programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB > 1 ? fmt::format("{} dB", programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB) : "Not Applicable (e.g. AOC) / Not known"
        );
    }
    if(fiberMode || copperMode) str.append("\n");



    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (passive copper or active cable) [146]", programming.byte_146_length_copper_in_1m_or_om4_in_2m == 0xFF ? " > 254 m" : (programming.byte_146_length_copper_in_1m_or_om4_in_2m > 1 ? fmt::format("{} m", programming.byte_146_length_copper_in_1m_or_om4_in_2m) : "Not Applicable (Not a cable assembly) / must be determined from separable free side device")
        );
    }
    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (OM4 50 um) [146]", programming.byte_146_length_copper_in_1m_or_om4_in_2m == 0xFF ? " > 508 m" : (programming.byte_146_length_copper_in_1m_or_om4_in_2m > 1 ? fmt::format("{} m", programming.byte_146_length_copper_in_1m_or_om4_in_2m * 2) : "Not Applicable (e.g. AOC) / OM4 fiber type not supported / must be determined from the free side device")
        );
    }
    if(fiberMode || copperMode) str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Device & Transmitter technology [147, 7-4]", getSFF8636_Transmitter_Technology_bit_7_4Info(programming.byte_147_device_technology_and_transmitter_technology.transmitter_Technology_bit_7_4).description
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Device & Transmitter technology [147, 3]", programming.byte_147_device_technology_and_transmitter_technology.wavelength_control_bit_3 ? "Active wavelength control" : "No wavelength control"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Device & Transmitter technology [147, 2]", programming.byte_147_device_technology_and_transmitter_technology.cooled_transmitter_bit_2 ? "Cooled transmitter device" : "Unooled transmitter"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Device & Transmitter technology [147, 1]", programming.byte_147_device_technology_and_transmitter_technology.pin_apd_detector_bit_1 ? "APD detector" : "Pin detector"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Device & Transmitter technology [147, 0]", programming.byte_147_device_technology_and_transmitter_technology.transmitter_tunable_bit_0 ? "Transmitter tunable" : "Transmitter not tunable"
    );
    str.append("\n");


    bool vendorNamePrintable = std::all_of(
        programming.byte_148_163_vendor_name.begin(),
        programming.byte_148_163_vendor_name.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorNamePrintable) {
        std::string vendorName = std::string(reinterpret_cast<char const *>(programming.byte_148_163_vendor_name.data()), programming.byte_148_163_vendor_name.size());
        //rtrim
        vendorName.erase(std::find_if(vendorName.rbegin(), vendorName.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorName.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor Name (wrapping quotes added by TransceiverTool) [148-163]", vendorName
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor Name (contains unprintable characters, printed as hex bytes) [148-163]"
        );

        for(int index = 0; index < programming.byte_148_163_vendor_name.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_148_163_vendor_name[index]);
        }
        str.append("\n");
    }
    str.append("\n");
    


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 7]", formatReservedBit(programming.byte_164_extended_module_codes.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 6]", formatReservedBit(programming.byte_164_extended_module_codes.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 5]", programming.byte_164_extended_module_codes.HDR_bit_5 ? "Supports HDR (200G) Infiniband" : "No support for HDR (200G) Infiniband"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 4]", programming.byte_164_extended_module_codes.EDR_bit_4 ? "Supports EDR (100G) Infiniband" : "No support for EDR (100G) Infiniband"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 3]", programming.byte_164_extended_module_codes.FDR_bit_3 ? "Supports FDR (56G) Infiniband" : "No support for FDR (56G) Infiniband"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 2]", programming.byte_164_extended_module_codes.QDR_bit_2 ? "Supports QDR (40G) Infiniband" : "No support for QDR (40G) Infiniband"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 1]", programming.byte_164_extended_module_codes.DDR_bit_1 ? "Supports DDR (20G) Infiniband" : "No support for DDR (20G) Infiniband"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Module Codes [164, 0]", programming.byte_164_extended_module_codes.SDR_bit_0 ? "Supports SDR (10G) Infiniband" : "No support for SDR (10G) Infiniband"
    );
    str.append("\n");




    auto VendorOUIsIt = std::find_if(TransceiverTool::VendorOUIs.begin(), TransceiverTool::VendorOUIs.end(), [seek = programming.byte_165_167_vendor_oui](const VendorOUI elem) { return elem.byte_value == seek; });
    std::string VendorOUIStr;
    if(VendorOUIsIt != TransceiverTool::VendorOUIs.end()) {
        VendorOUIStr = fmt::format("Vendor name is {:?}", VendorOUIsIt->name);
    } else {
        VendorOUIStr = "Vendor name is unknown";
    }
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Vendor OUI [165-167]", fmt::format("{:02x}:{:02x}:{:02x} ({})", programming.byte_165_167_vendor_oui[0], programming.byte_165_167_vendor_oui[1], programming.byte_165_167_vendor_oui[2], VendorOUIStr)
    );
    str.append("\n");




    bool vendorPNPrintable = std::all_of(
        programming.byte_168_183_vendor_pn.begin(),
        programming.byte_168_183_vendor_pn.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorPNPrintable) {
        std::string vendorPN = std::string(reinterpret_cast<char const *>(programming.byte_168_183_vendor_pn.data()), 16);
        //rtrim
        vendorPN.erase(std::find_if(vendorPN.rbegin(), vendorPN.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorPN.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor PN (wrapping quotes added by TransceiverTool) [168-183]", vendorPN
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor PN (contains unprintable characters, printed as hex bytes) [168-183]"
        );

        for(int index = 0; index < programming.byte_168_183_vendor_pn.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_168_183_vendor_pn[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    bool vendorRevPrintable = std::all_of(
        programming.byte_184_185_vendor_rev.begin(),
        programming.byte_184_185_vendor_rev.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorRevPrintable) {
        std::string vendorRev = std::string(reinterpret_cast<char const *>(programming.byte_184_185_vendor_rev.data()), 2);
        //rtrim
        vendorRev.erase(std::find_if(vendorRev.rbegin(), vendorRev.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorRev.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor Rev (wrapping quotes added by TransceiverTool) [184-185]", vendorRev
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor Rev (contains unprintable characters, printed as hex bytes) [184-185]"
        );

        for(int index = 0; index < programming.byte_184_185_vendor_rev.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_184_185_vendor_rev[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    if(fiberMode) {
        static_assert(sizeof(unsigned int) >= 2);
        unsigned int wavelengthRaw = 0;
        wavelengthRaw |= (unsigned(programming.byte_186_wavelength_high_order_or_copper_attenuation) << 8);
        wavelengthRaw |= (unsigned(programming.byte_187_wavelength_low_order_or_copper_attenuation) << 0);

        double wavelength = double(wavelengthRaw) / 20.0;

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Wavelength [186-187]", fmt::format("{} nm", wavelength)
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 2.5GHz [186]", fmt::format("{} dB", programming.byte_186_wavelength_high_order_or_copper_attenuation)
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 5.0GHz [187]", fmt::format("{} dB", programming.byte_187_wavelength_low_order_or_copper_attenuation)
        );
    }
    if(fiberMode || copperMode) str.append("\n");

    if(fiberMode) {
        static_assert(sizeof(unsigned int) >= 2);
        unsigned int wavelengthToleranceRaw = 0;
        wavelengthToleranceRaw |= (unsigned(programming.byte_188_wavelength_tolerance_high_order_or_copper_attenuation) << 8);
        wavelengthToleranceRaw |= (unsigned(programming.byte_189_wavelength_tolerance_low_order_or_copper_attenuation) << 0);

        double wavelengthTolerance = double(wavelengthToleranceRaw) / 200.0;

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Wavelength Tolerance [188-189]", fmt::format("{} nm", wavelengthTolerance)
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 7.0GHz [188]", fmt::format("{} dB", programming.byte_188_wavelength_tolerance_high_order_or_copper_attenuation)
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 12.9GHz [189]", fmt::format("{} dB", programming.byte_189_wavelength_tolerance_low_order_or_copper_attenuation)
        );
    }
    if(fiberMode || copperMode) str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Max case temperature [190]", fmt::format("{} deg C", programming.byte_190_max_case_temperature == 0 ? 70 : programming.byte_190_max_case_temperature)
    );
    str.append("\n");


    if(programming.byte_191_CC_BASE == CC_BASE) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_BASE checksum [191]", fmt::format("{:#04x} (correct)", programming.byte_191_CC_BASE)
        );
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_BASE checksum [191]", fmt::format("{:#04x} (incorrect, should be {:#04x})", programming.byte_191_CC_BASE, CC_BASE)
        );
    }
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Specification Compliance [192]", SFF8024::byteToExtendedComplianceCodeString(programming.byte_192_extended_specification_compliance_codes)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 7]", formatReservedBit(programming.byte_193_option_values.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 6]", programming.byte_193_option_values.lpmode_txdis_input_configurable_bit_6 ? "LPMode/TxDis input signal is configurable" : "LPMode/TxDis input signal NOT configurable"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 5]", programming.byte_193_option_values.intl_rxlosl_output_configurable_bit_5 ? "IntL/RxLOSL output signal is configurable" : "IntL/RxLOSL output signal NOT configurable"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 4]", programming.byte_193_option_values.tx_input_adaptive_equalizers_freeze_capable_bit_4 ? "Tx input adaptive equalizers freeze capable" : "Tx input adaptive equalizers NOT freeze capable"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 3]", programming.byte_193_option_values.tx_input_equalizers_auto_adaptive_capable_bit_3 ? "Tx input equalizers auto-adaptive capable" : "Tx input equalizers NOT auto-adaptive capable"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 2]", programming.byte_193_option_values.tx_input_equalizers_fixed_programmable_settings_bit_2 ? "Tx input equalizers fixed-programmable settings implemented" : "Tx input equalizers fixed-programmable settings NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 1]", programming.byte_193_option_values.rx_output_emphasis_fixed_programmable_settings_bit_1 ? "Rx output emphasis fixed-programmable settings implemented" : "Rx output emphasis fixed-programmable settings NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [193, 0]", programming.byte_193_option_values.rx_output_amplitude_fixed_programmable_settings_bit_0 ? "Rx output amplitude fixed-programmable settings implemented" : "Rx output amplitude fixed-programmable settings NOT implemented"
    );
    str.append("\n");




    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 7]", programming.byte_194_option_values.tx_cdr_on_off_control_implemented_bit_7 ? "Tx CDR On/Off Control implemented" : "Tx CDR On/Off Control NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 6]", programming.byte_194_option_values.rx_cdr_on_off_control_implemented_bit_6 ? "Rx CDR On/Off Control implemented" : "Rx CDR On/Off Control NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 5]", programming.byte_194_option_values.tx_cdr_loss_of_lock_lol_flag_implemented_bit_5 ? "Tx CDR Loss of Lock (LOL) flag implemented" : "Tx CDR Loss of Lock (LOL) flag NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 4]", programming.byte_194_option_values.rx_cdr_loss_of_lock_lol_flag_implemented_bit_4 ? "Rx CDR Loss of Lock (LOL) flag implemented" : "Rx CDR Loss of Lock (LOL) flag NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 3]", programming.byte_194_option_values.rx_squelch_disable_implemented_bit_3 ? "Rx Squelch Disable implemented" : "Rx Squelch Disable NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 2]", programming.byte_194_option_values.rx_output_disable_implemented_bit_2 ? "Rx Output Disable implemented" : "Rx Output Disable NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 1]", programming.byte_194_option_values.tx_squelch_disable_implemented_bit_1 ? "Tx Squelch Disable implemented" : "Tx Squelch Disable NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [194, 0]", programming.byte_194_option_values.tx_squelch_implemented_bit_0 ? "Tx Squelch implemented" : "Tx Squelch NOT implemented"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 7]", programming.byte_195_option_values.memory_page_02_provided_bit_7 ? "Memory Page 02h provided" : "Memory Page 02h NOT provided"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 6]", programming.byte_195_option_values.memory_page_01h_provided_bit_6 ? "Memory Page 01h provided" : "Memory Page 01h NOT provided"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 5]", programming.byte_195_option_values.rate_select_implemented_bit_5 ? "Rate select is implemented as defined in 6.2.7" : "Rate select NOT implemented as defined in 6.2.7"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 4]", programming.byte_195_option_values.tx_disable_implemented_bit_4 ? "Tx_Disable is implemented" : "Tx_Disable is NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 3]", programming.byte_195_option_values.tx_fault_signal_implemented_bit_3 ? "Tx_Fault signal implemented" : "Tx_Fault signal NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 2]", programming.byte_195_option_values.tx_squelch_implemented_to_reduce_pave_bit_2 ? "Tx Squelch implemented to reduce Pave" : "Tx Squelch implemented to reduce OMA"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 1]", programming.byte_195_option_values.tx_loss_of_signal_implemented_bit_1 ? "Tx Loss of Signal implemented" : "Tx Loss of Signal NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Options [195, 0]", programming.byte_195_option_values.pages_20_to_21h_implemented_bit_0 ? "Pages 20-21h implemented" : "Pages 20-21h NOT implemented"
    );
    str.append("\n");



    bool vendorSerialNumberPrintable = std::all_of(
        programming.byte_196_211_vendor_sn.begin(),
        programming.byte_196_211_vendor_sn.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorSerialNumberPrintable) {
        std::string vendorSerialNumber = std::string(reinterpret_cast<char const *>(programming.byte_196_211_vendor_sn.data()), 16);
        //rtrim
        vendorSerialNumber.erase(std::find_if(vendorSerialNumber.rbegin(), vendorSerialNumber.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorSerialNumber.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor S/N (wrapping quotes added by TransceiverTool) [196-211]", vendorSerialNumber
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor S/N (contains unprintable characters, printed as hex bytes) [196-211]"
        );

        for(int index = 0; index < programming.byte_196_211_vendor_sn.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_196_211_vendor_sn[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    bool dateCodeYearPrintable = std::all_of(
        programming.byte_212_219_date_code.year_low_order_digits.begin(),
        programming.byte_212_219_date_code.year_low_order_digits.end(),
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeYearPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, low order digits of year [212-213]", std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.year_low_order_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, low order digits of year (invalid format, printed as hex bytes) [212-213]"
        );

        for(int index = 0; index < programming.byte_212_219_date_code.year_low_order_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_212_219_date_code.year_low_order_digits[index]);
        }
        str.append("\n");
    }

    bool dateCodeMonthPrintable = std::all_of(
        programming.byte_212_219_date_code.month_digits.begin(),
        programming.byte_212_219_date_code.month_digits.end(), 
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeMonthPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, digits of month [214-215]", std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.month_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, digits of month (invalid format, printed as hex bytes) [214-215]"
        );

        for(int index = 0; index < programming.byte_212_219_date_code.month_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_212_219_date_code.month_digits[index]);
        }
        str.append("\n");
    }

    bool dateCodeDayPrintable = std::all_of(
        programming.byte_212_219_date_code.day_digits.begin(),
        programming.byte_212_219_date_code.day_digits.end(),
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeDayPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, digits of day [216-217]", std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.day_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, digits of day (invalid format, printed as hex bytes) [216-217]"
        );

        for(int index = 0; index < programming.byte_212_219_date_code.day_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_212_219_date_code.day_digits[index]);
        }
        str.append("\n");
    }

    bool lotCodePrintable = std::all_of(programming.byte_212_219_date_code.lot_code.begin(), programming.byte_212_219_date_code.lot_code.end(), [](char c) {return std::isprint(c); });
    if(dateCodeDayPrintable) {
        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Date Code, lot code (wrapping quotes added by TransceiverTool) [218-219]", std::string(reinterpret_cast<char const *>(programming.byte_212_219_date_code.lot_code.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, lot code (invalid format, printed as hex bytes) [218-219]"
        );

        for(int index = 0; index < programming.byte_212_219_date_code.lot_code.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_212_219_date_code.lot_code[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 7]", formatReservedBit(programming.byte_220_diagnostic_monitoring_type.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 6]", formatReservedBit(programming.byte_220_diagnostic_monitoring_type.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 5]", programming.byte_220_diagnostic_monitoring_type.temperature_monitoring_implemented_bit_5 ? "Temperature monitoring implemented" : "Temperature monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 4]", programming.byte_220_diagnostic_monitoring_type.supply_voltage_monitoring_implemented_bit_4 ? "Supply voltage monitoring implemented" : "Supply voltage monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 3]", programming.byte_220_diagnostic_monitoring_type.received_power_measurement_is_average_bit_3 ? "Received power measurements type = Average" : "Received power measurements type = OMA"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 2]", programming.byte_220_diagnostic_monitoring_type.transmitter_power_measurement_supported_bit_2 ? "Transmitter power measurement supported" : "Transmitter power measurement NOT supported"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 1]", formatReservedBit(programming.byte_220_diagnostic_monitoring_type.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Diagnostic Monitoring Type [220, 0]", formatReservedBit(programming.byte_220_diagnostic_monitoring_type.reserved_bit_0)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 7]", formatReservedBit(programming.byte_221_enhanced_options.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 6]", formatReservedBit(programming.byte_221_enhanced_options.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 5]", formatReservedBit(programming.byte_221_enhanced_options.reserved_bit_5)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 4]", programming.byte_221_enhanced_options.initialization_complete_flag_implemented_bit_4 ? "Initialization Complete Flag implemented" : "Initialization Complete Flag NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 3]", programming.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3 ? "Rate selection is implemented using extended rate selection" : "Rate selection is NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 2]", formatReservedBit(programming.byte_221_enhanced_options.reserved_bit_2)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 1]", programming.byte_221_enhanced_options.readiness_flag_implemented_bit_1 ? "TC readiness flag implemented" : "TC readiness flag NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [221, 0]", programming.byte_221_enhanced_options.software_reset_implemented_bit_0 ? "Software reset is implemented" : "Software reset is NOT implemented"
    );
    str.append("\n");
    

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Baud Rate, nominal [222]", fmt::format("{} MBd", unsigned(programming.byte_222_extended_baud_rate_in_250_mbaud) * 250)
    );
    str.append("\n");


    if(programming.byte_223_CC_EXT == CC_EXT) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_EXT checksum [223]", fmt::format("{:#04x} (correct)", programming.byte_223_CC_EXT)
        );
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_EXT checksum [223]", fmt::format("{:#04x} (incorrect, should be {:#04x})", programming.byte_223_CC_EXT, CC_EXT)
        );
    }
    str.append("\n");


    bool vendorExtensionPrintable = std::all_of(
        programming.byte_224_255_vendor_specific.begin(),
        programming.byte_224_255_vendor_specific.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorExtensionPrintable) {
        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor Specific (wrapping quotes added by TransceiverTool) [224-255]", std::string(reinterpret_cast<char const *>(programming.byte_224_255_vendor_specific.data()), 32)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor Specific (nonprintable, printed as hex bytes) [224-255]"
        );

        for(int index = 0; index < programming.byte_224_255_vendor_specific.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_224_255_vendor_specific[index]);
        }
        str.append("\n");
    }
    str.append("\n");

    return str;

}