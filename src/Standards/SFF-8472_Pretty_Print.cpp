#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8024_Encoding_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Extended_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "TransceiverTool/Standards/SFF-8472_Rate_Identifiers.hpp"
#include "TransceiverTool/Standards/SFF-8472_Checksum.hpp"
#include "TransceiverTool/Standards/common.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include <algorithm>
#include <iterator>
#include "TransceiverTool/Vendor_OUIs.hpp"
#include <vector>


std::string TransceiverTool::Standards::SFF8472::prettyPrintProgramming(const SFF8472_LowerA0h &programming, bool fiberMode, bool copperMode) {

    std::string optionTitleFormatString = "{: <85s}: {:s}\n";
    auto formatReservedBit = [](bool bit) -> std::string {
        if(bit) {
            return fmt::format("{}", fmt::styled("Reserved bit set", fmt::bg(fmt::color::red)));
        } else {
            return "N/A";
        }
    };
    
    std::vector<unsigned char> binary; binary.resize(128, 0x00);
    assembleToBinary(binary.data(), programming, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING, common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING);

    unsigned char CC_BASE = calculateCC_BASEChecksum(binary.data());
    unsigned char CC_EXT = calculateCC_EXTChecksum(binary.data());


    std::string str;

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Type", "SFF-8472");
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Identifier [0]", SFF8472::byteToPhysicalDeviceIdentifierString(programming.byte_0_Identifier));
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, "Extended Identifier [1]", SFF8472::byteToPhysicalDeviceExtendedIdentifierString(programming.byte_1_extended_identifier));
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Connector Type [2]", SFF8024::byteToTransceiverConnectorTypeString(programming.byte_2_Connector_type)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 7]", programming.byte_3_ethernet_compliance_codes._10GBASE_ER_bit_7 ? "10GBASE-ER compliant" : "Not 10GBASE-ER compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 6]", programming.byte_3_ethernet_compliance_codes._10GBASE_LRM_bit_6 ? "10GBASE-LRM compliant" : "Not 10GBASE-LRM compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 5]", programming.byte_3_ethernet_compliance_codes._10GBASE_LR_bit_5 ? "10GBASE-LR compliant" : "Not 10GBASE-LR compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, 10G Ethernet Compliance Codes [3, 4]", programming.byte_3_ethernet_compliance_codes._10GBASE_SR_bit_4 ? "10GBASE-SR compliant" : "Not 10GBASE-SR compliant"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 3]", programming.byte_3_infiniband_compliance_codes._1X_SX_bit_3 ? "1X SX compliant" : "Not 1X SX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 2]", programming.byte_3_infiniband_compliance_codes._1X_LX_bit_2 ? "1X LX compliant" : "Not 1X LX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 1]", programming.byte_3_infiniband_compliance_codes._1X_Copper_Active_bit_1 ? "1X Copper Active compliant" : "Not 1X Copper Active compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Infiniband Compliance Codes [3, 0]", programming.byte_3_infiniband_compliance_codes._1X_Copper_Passive_bit_0 ? "1X Copper Passive compliant" : "Not 1X Copper Passive compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, ESCON Compliance Codes [4, 7]", programming.byte_4_escon_compliance_codes.ESCON_MMF_1310nm_LED_bit_7 ? "ESCON MMF, 1310nm LED compliant" : "Not ESCON MMF, 1310nm LED compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, ESCON Compliance Codes [4, 6]", programming.byte_4_escon_compliance_codes.ESCON_SMF_1310nm_Laser_bit_6 ? "ESCON SMF, 1310nm Laser compliant" : "Not ESCON SMF, 1310nm Laser compliant"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 5]", programming.byte_4_5_sonet_compliance_codes.OC_192_short_reach_byte_4_bit_5 ? "OC-192, short reach compliant" : "Not OC-192, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 4-3]", 
        fmt::format(
            "{} ({:#04b})", 
            getSFF8472_SONETReachSpecifierInfo(programming.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3).name,
            getSFF8472_SONETReachSpecifierInfo(programming.byte_4_5_sonet_compliance_codes.SONET_Reach_Specifier_byte_4_bit_4_3).byte_value
        )
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 2]", programming.byte_4_5_sonet_compliance_codes.OC_48_long_reach_byte_4_bit_2 ? "OC-48, long reach compliant" : "Not OC-48, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 1]", programming.byte_4_5_sonet_compliance_codes.OC_48_intermediate_reach_byte_4_bit_1 ? "OC-48, intermediate reach compliant" : "Not OC-48, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [4, 0]", programming.byte_4_5_sonet_compliance_codes.OC_48_short_reach_byte_4_bit_0 ? "OC-48, short reach compliant" : "Not OC-48, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 7]", formatReservedBit(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 6]", programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_long_reach_2_byte_5_bit_6 ? "OC-12, single mode, long reach compliant" : "Not OC-12, single mode, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 5]", programming.byte_4_5_sonet_compliance_codes.OC_12_single_mode_intermediate_reach_byte_5_bit_5 ? "OC-12, single mode, intermediate reach compliant" : "Not OC-12, single mode, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 4]", programming.byte_4_5_sonet_compliance_codes.OC_12_short_reach_byte_5_bit_4 ? "OC-12, short reach compliant" : "Not OC-12, short reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 3]", formatReservedBit(programming.byte_4_5_sonet_compliance_codes.reserved_byte_5_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 2]", programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_long_reach_byte_5_bit_2 ? "OC-3, single mode, long reach compliant" : "Not OC-3, single mode, long reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 1]", programming.byte_4_5_sonet_compliance_codes.OC_3_single_mode_intermediate_reach_byte_5_bit_1 ? "OC-3, single mode, intermediate reach compliant" : "Not OC-3, single mode, intermediate reach compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SONET Compliance Codes [5, 0]", programming.byte_4_5_sonet_compliance_codes.OC_3_short_reach__byte_5_bit_0 ? "OC-3, short reach compliant" : "Not OC-3, short reach compliant"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 7]", programming.byte_6_ethernet_compliance_codes.BASE_PX_bit_7 ? "BASE-PX compliant" : "Not BASE-PX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 6]", programming.byte_6_ethernet_compliance_codes.BASE_BX10_bit_6 ? "BASE-BX10 compliant" : "Not BASE-BX10 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 5]", programming.byte_6_ethernet_compliance_codes._100BASE_FX_bit_5 ? "100BASE-FX compliant" : "Not 100BASE-FX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 4]", programming.byte_6_ethernet_compliance_codes._100BASE_LX_LX10_bit_4 ? "100BASE-LX/LX10 compliant" : "Not 100BASE-LX/LX10 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 3]", programming.byte_6_ethernet_compliance_codes._1000BASE_T_bit_3 ? "1000BASE-T compliant" : "Not 1000BASE-T compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 2]", programming.byte_6_ethernet_compliance_codes._1000BASE_CX_bit_2 ? "1000BASE-CX compliant" : "Not 1000BASE-CX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 1]", programming.byte_6_ethernet_compliance_codes._1000BASE_LX_bit_1 ? "1000BASE-LX compliant" : "Not 1000BASE-LX compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Ethernet Compliance Codes [6, 0]", programming.byte_6_ethernet_compliance_codes._1000BASE_SX_bit_0 ? "1000BASE-SX compliant" : "Not 1000BASE-SX compliant"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [7, 7]", programming.byte_7_fibre_channel_link_length_codes.very_long_distance_v_bit_7 ? "very long distance (V) compliant" : "Not very long distance (V) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [7, 6]", programming.byte_7_fibre_channel_link_length_codes.short_distance_s_bit_6 ? "short distance (S) compliant" : "Not short distance (S) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [7, 5]", programming.byte_7_fibre_channel_link_length_codes.intermediate_distance_i_bit_5 ? "intermediate distance (I) compliant" : "Not intermediate distance (I) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [7, 4]", programming.byte_7_fibre_channel_link_length_codes.long_distance_l_bit_4 ? "long distance (L) compliant" : "Not long distance (L) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Link Length [7, 3]", programming.byte_7_fibre_channel_link_length_codes.medium_distance_m_bit_3 ? "medium distance (M) compliant" : "Not medium distance (M) compliant"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [7, 2]", programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_linear_Rx_SA_byte_7_bit_2 ? "Shortwave laser, linear Rx (SA) compliant" : "Not Shortwave laser, linear Rx (SA) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [7, 1]", programming.byte_7_8_fibre_channel_technology_codes.Longwave_laser_LC_byte_7_bit_1 ? "Longwave laser (LC) compliant" : "Not Longwave laser (LC) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [7, 0]", programming.byte_7_8_fibre_channel_technology_codes.Electrical_inter_enclosure_EL_byte_7_bit_0 ? "Electrical inter-enclosure (EL) compliant" : "Not Electrical inter-enclosure (EL) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [8, 7]", programming.byte_7_8_fibre_channel_technology_codes.Electrical_intra_enclosure_EL_byte_byte_8_bit_7 ? "Electrical intra-enclosure (EL) compliant" : "Not Electrical intra-enclosure (EL) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [8, 6]", programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_w_o_OFC_SN_byte_8_bit_6 ? "Shortwave laser w/o OFC (SN) compliant" : "Not Shortwave laser w/o OFC (SN) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [8, 5]", programming.byte_7_8_fibre_channel_technology_codes.Shortwave_laser_with_OFC_SL_byte_8_bit_5 ? "Shortwave laser with OFC (SL) 4 compliant" : "Not Shortwave laser with OFC (SL) 4 compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Technology [8, 4]", programming.byte_7_8_fibre_channel_technology_codes.Longwave_Laser_LL_byte_8_bit_4 ? "Longwave laser (LL) compliant" : "Not Longwave laser (LL) compliant"
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SFP+ Cable Technology [8, 3]", programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3 ? "Active Cable compliant" : "Not Active Cable compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SFP+ Cable Technology [8, 2]", programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 ? "Passive Cable compliant" : "Not Passive Cable compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SFP+ Cable Technology [8, 1]", formatReservedBit(programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, SFP+ Cable Technology [8, 0]", formatReservedBit(programming.byte_8_sfp_plus_cable_technology_codes.reserved_bit_0)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 7]", programming.byte_9_fibre_channel_transmission_media_codes.Twin_Axial_Pair_TW_bit_7 ? "Twin Axial Pair (TW) compliant" : "Not Twin Axial Pair (TW) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 6]", programming.byte_9_fibre_channel_transmission_media_codes.Twisted_Pair_TP_bit_6 ? "Twisted Pair (TP) compliant" : "Not Twisted Pair (TP) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 5]", programming.byte_9_fibre_channel_transmission_media_codes.Miniature_Coax_MI_bit_5 ? "Miniature Coax (MI) compliant" : "Not Miniature Coax (MI) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 4]", programming.byte_9_fibre_channel_transmission_media_codes.Video_Coax_TV_bit_4 ? "Video Coax (TV) compliant" : "Not Video Coax (TV) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 3]", programming.byte_9_fibre_channel_transmission_media_codes.Multimode_62_5_um_M6_bit_3 ? "Multimode, 62.5um (M6) compliant" : "Not Multimode, 62.5um (M6) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 2]", programming.byte_9_fibre_channel_transmission_media_codes.Multimode_50_um_M5_M5E_bit_2 ? "Multimode, 50um (M5, M5E) compliant" : "Not Multimode, 50um (M5, M5E) compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 1]", formatReservedBit(programming.byte_9_fibre_channel_transmission_media_codes.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Transmission Media [9, 0]", programming.byte_9_fibre_channel_transmission_media_codes.Single_Mode_SM_bit_0 ? "Single Mode (SM) compliant" : "Not Single Mode (SM) compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 7]", programming.byte_10_fibre_channel_speed_codes._1200_MBytes_sec_bit_7 ? "1200 MBytes/sec compliant" : "Not 1200 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 6]", programming.byte_10_fibre_channel_speed_codes._800_MBytes_sec_bit_6 ? "800 MBytes/sec compliant" : "Not 800 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 5]", programming.byte_10_fibre_channel_speed_codes._1600_MBytes_sec_bit_5 ? "1600 MBytes/sec compliant" : "Not 1600 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 4]", programming.byte_10_fibre_channel_speed_codes._400_MBytes_sec_bit_4 ? "400 MBytes/sec compliant" : "Not 400 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 3]", programming.byte_10_fibre_channel_speed_codes._3200_MBytes_sec_bit_3 ? "3200 MBytes/sec compliant" : "Not 3200 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 2]", programming.byte_10_fibre_channel_speed_codes._200_MBytes_sec_bit_2 ? "200 MBytes/sec compliant" : "Not 200 MBytes/sec compliant"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 1]", programming.byte_10_fibre_channel_speed_codes.see_byte_62_fibre_channel_speed_2_bit_1 ? "See byte 62 \"Fibre Channel Speed 2\"" : "No \"Fibre Channel Speed 2\""
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed [10, 0]", programming.byte_10_fibre_channel_speed_codes._100_MBytes_sec_bit_0 ? "100 MBytes/sec compliant" : "Not 100 MBytes/sec compliant"
    );
    str.append("\n");



    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Encoding [11]", SFF8024::byteToSFF8472TransceiverEncodingString(programming.byte_11_Encoding)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Signaling rate, nominal [12]", 
            programming.byte_12_nominal_signaling_rate_in_100_mbaud != 0xFF ? 
                (programming.byte_12_nominal_signaling_rate_in_100_mbaud != 0x00 ? 
                    fmt::format("{} MBd", unsigned(programming.byte_12_nominal_signaling_rate_in_100_mbaud) * 100) : 
                    "Not Specified"
                ) :
            "> 25.4GBd (See Bytes 66-67)"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Rate Identifier [13]", byteToRateIdentifierString(programming.byte_13_rate_identifier)
    );
    str.append("\n");


    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (SMF) [14]", (programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 255 ? "> 254 km" : fmt::format("{} km", programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz))
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 12.9 GHz [14]", (programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz == 0) ? "not known / unavailable" : fmt::format("{} dB", programming.byte_14_length_smf_in_kilometers_or_copper_attenuation_in_db_at_12_9_ghz)
        );
    }
    if(copperMode || fiberMode) str.append("\n");

    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (SMF) [15]", (programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 255 ? "> 25.4 km" : fmt::format("{} m", (unsigned)(programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz) * 100u))
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Cable Attenuation @ 25.78 GHz [15]", (programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz == 0) ? "not known / unavailable" : fmt::format("{} dB", programming.byte_15_length_smf_in_100_m_or_copper_attenuation_in_db_at_25_78_ghz)
        );
    }
    if(copperMode || fiberMode) str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Length (OM2 50 um) [16]", (programming.byte_16_length_om2_in_10_m == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_16_length_om2_in_10_m == 255 ? "> 2.54 km" : fmt::format("{} m", unsigned(programming.byte_16_length_om2_in_10_m) * 10u))
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Length (OM1 62.5 um) [17]", (programming.byte_17_length_om1_in_10_m == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_17_length_om1_in_10_m == 255 ? "> 2.54 km" : fmt::format("{} m", unsigned(programming.byte_17_length_om1_in_10_m) * 10u))
    );
    str.append("\n");


    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (OM4 50 um) [18]", (programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 255 ? "> 2.54 km" : fmt::format("{} m", (unsigned)(programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m) * 10u))
        );
    }
    if(copperMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Maximum Link Length or Actual Active Cable Length [18]", (programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m == 255 ? "> 254 m" : fmt::format("{} m", programming.byte_18_link_length_om4_10m_or_copper_or_dac_length_in_m))
        );
    }
    if(copperMode || fiberMode) str.append("\n");


    if(fiberMode) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Length (OM3 50 um) [19]", (programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value == 0) ? "not supported / must be determined from transceiver technology" : (programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value == 255 ? "> 2.54 km" : fmt::format("{} m", (unsigned)(programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value) * 10u))
        );
    }
    if(copperMode) {
        unsigned char multiplierBits = (programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value & 0b11000000) >> 6;
        unsigned char baseValue = programming.byte_19_length_om3_in_10m_or_copper_or_dac_multiplier_and_base_value & 0b00111111;

        double multiplier;
        if(multiplierBits == 0b00) { multiplier = 0.1; }
        else if(multiplierBits == 0b01) { multiplier = 1.0; }
        else if(multiplierBits == 0b10) { multiplier = 10.0; }
        else {multiplier = 100.0;}
        
        double actualLength = double(baseValue) * multiplier;

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Copper Maximum Link Length or Actual Active Cable Length [19]", fmt::format("{} m ({} m * {})", actualLength, baseValue, multiplier)
        );
    }
    if(copperMode || fiberMode) str.append("\n");

    bool vendorNamePrintable = std::all_of(
        programming.byte_20_35_vendor_name.begin(),
        programming.byte_20_35_vendor_name.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorNamePrintable) {
        std::string vendorName = std::string(reinterpret_cast<char const *>(programming.byte_20_35_vendor_name.data()), programming.byte_20_35_vendor_name.size());
        //rtrim
        vendorName.erase(std::find_if(vendorName.rbegin(), vendorName.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorName.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor Name (wrapping quotes added by TransceiverTool) [20-35]", vendorName
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor Name (contains unprintable characters, printed as hex bytes) [20-35]"
        );

        for(int index = 0; index < programming.byte_20_35_vendor_name.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_20_35_vendor_name[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Extended Specification Compliance [36]", SFF8024::byteToExtendedComplianceCodeString(programming.byte_36_extended_specification_compliance_codes)
    );
    str.append("\n");

    auto VendorOUIsIt = std::find_if(TransceiverTool::VendorOUIs.begin(), TransceiverTool::VendorOUIs.end(), [seek = programming.byte_37_39_vendor_oui](const VendorOUI elem) { return elem.byte_value == seek; });
    std::string VendorOUIStr;
    if(VendorOUIsIt != TransceiverTool::VendorOUIs.end()) {
        VendorOUIStr = fmt::format("Vendor name is {:?}", VendorOUIsIt->name);
    } else {
        VendorOUIStr = "Vendor name is unknown";
    }
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Vendor OUI [37-39]", fmt::format("{:02x}:{:02x}:{:02x} ({})", programming.byte_37_39_vendor_oui[0], programming.byte_37_39_vendor_oui[1], programming.byte_37_39_vendor_oui[2], VendorOUIStr)
    );
    str.append("\n");


    bool vendorPNPrintable = std::all_of(
        programming.byte_40_55_vendor_pn.begin(),
        programming.byte_40_55_vendor_pn.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorPNPrintable) {
        std::string vendorPN = std::string(reinterpret_cast<char const *>(programming.byte_40_55_vendor_pn.data()), 16);
        //rtrim
        vendorPN.erase(std::find_if(vendorPN.rbegin(), vendorPN.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorPN.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor PN (wrapping quotes added by TransceiverTool) [40-55]", vendorPN
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor PN (contains unprintable characters, printed as hex bytes) [40-55]"
        );

        for(int index = 0; index < programming.byte_40_55_vendor_pn.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_40_55_vendor_pn[index]);
        }
        str.append("\n");
    }
    str.append("\n");

    bool vendorRevPrintable = std::all_of(
        programming.byte_56_59_vendor_rev.begin(),
        programming.byte_56_59_vendor_rev.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorRevPrintable) {
        std::string vendorRev = std::string(reinterpret_cast<char const *>(programming.byte_56_59_vendor_rev.data()), 4);
        //rtrim
        vendorRev.erase(std::find_if(vendorRev.rbegin(), vendorRev.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorRev.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor Rev (wrapping quotes added by TransceiverTool) [56-59]", vendorRev
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor Rev (contains unprintable characters, printed as hex bytes) [56-59]"
        );

        for(int index = 0; index < programming.byte_56_59_vendor_rev.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_56_59_vendor_rev[index]);
        }
        str.append("\n");
    }
    str.append("\n");

    if(!programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && !programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
        static_assert(sizeof(unsigned int) >= 2);
        unsigned int wavelengthRaw = 0;
        wavelengthRaw |= (unsigned(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance) << 8);
        wavelengthRaw |= (unsigned(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance) << 0);

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Wavelength [60-61]", fmt::format("{} nm", wavelengthRaw)
        );
    } else if(programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && !programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 7]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 6]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 5]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 4]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 3]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 3))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 2]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 2))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 1]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 1)) ? "FC-PI-4 Appendix H compliant" : "Not FC-PI-4 Appendix H compliant"
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [60, 0]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 0)) ? "SFF-8431 Appendix E compliant" : "Not SFF-8431 Appendix E compliant"
        );

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 7]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 6]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 5]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 4]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 3]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 2]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 1]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Passive Cable Specification Compliance [61, 0]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0))
        );
    } else if(!programming.byte_8_sfp_plus_cable_technology_codes.Passive_Cable_bit_2 && programming.byte_8_sfp_plus_cable_technology_codes.Active_Cable_bit_3){
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 7]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 7))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 6]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 6))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 5]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 5))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 4]", formatReservedBit(programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 4))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 3]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 3)) ? "FC-PI-4 Limiting compliant" : "Not FC-PI-4 Limiting compliant"
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 2]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 2)) ? "SFF-8431 Limiting compliant" : "Not SFF-8431 Limiting compliant"
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 1]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 1)) ? "FC-PI-4 Appendix H compliant" : "Not FC-PI-4 Appendix H compliant"
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [60, 0]", (programming.byte_60_wavelength_high_order_byte_or_cable_specification_compliance & (1 << 0)) ? "SFF-8431 Appendix E compliant" : "Not SFF-8431 Appendix E compliant"
        );

        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 7]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 7))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 6]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 6))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 5]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 5))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 4]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 4))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 3]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 3))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 2]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 2))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 1]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 1))
        );
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Active Cable Specification Compliance [61, 0]", formatReservedBit(programming.byte_61_wavelength_low_order_byte_or_cable_specification_compliance & (1 << 0))
        );
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Wavelength / Passive / Active Cable Specification Compliance [60-61]", "Cannot print, both Active and Passive cable bits are set"
        );
    }
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 7]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 6]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_6)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 5]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_5)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 4]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_4)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 3]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_3)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 2]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_2)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 1]", formatReservedBit(programming.byte_62_fibre_channel_2_speed_codes.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Specification Compliance, Fibre Channel Speed 2 [62, 0]", programming.byte_62_fibre_channel_2_speed_codes._64_GFC_bit_0 ? "64 GFC compliant" : "Not 64 GFC compliant"
    );
    str.append("\n");

    if(programming.byte_63_CC_BASE == CC_BASE) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_BASE checksum [63]", fmt::format("{:#04x} (correct)", programming.byte_63_CC_BASE)
        );
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_BASE checksum [63]", fmt::format("{:#04x} (incorrect, should be {:#04x})", programming.byte_63_CC_BASE, CC_BASE)
        );
    }
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 7]", formatReservedBit(programming.byte_64_option_values.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 6]", programming.byte_64_option_values.high_level_power_level_4_bit_6 ? "High Power Level (Level 4) requirement" : "No High Power Level (Level 4) requirement"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 5]", programming.byte_64_option_values.high_level_power_level_4_3_bit_5 ? "High Power Level (Level 3 or Level 4) requirement" : "No High Power Level (Level 3 or Level 4) requirement"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 4]", programming.byte_64_option_values.paging_implemented_indicator_bit_4 ? "Paging implemented" : "Paging NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 3]", programming.byte_64_option_values.retimer_or_cdr_indicator_bit_3 ? "Internal retimer or data recovery (CDR) circuit present" : "No internal retimer or data recovery (CDR) circuit present"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 2]", programming.byte_64_option_values.cooled_transceiver_bit_2 ? "Cooled Laser" : "Conventional Uncooled / Unspecified Laser"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 1]", programming.byte_64_option_values.power_level_2_bit_1 ? "Power Level 2 requirement" : "Power Level 1 (or unspecified) requirement"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [64, 0]", programming.byte_64_option_values.linear_receiver_output_implemented_bit_0 ? "Linear Receiver Output implemented" : "Conventional limiting, PAM4 or unspecified transceiver receiver output"
    );
    str.append("\n");

    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 7]", programming.byte_65_option_values.receiver_rdt_implemented_bit_7 ? "Receiver decision threshold implemented" : "Receiver decision threshold NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 6]", programming.byte_65_option_values.receiver_tunable_bit_6 ? "Transmitter tunable according to SFF-8690" : "Transmitter NOT tunable"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 5]", programming.byte_65_option_values.rate_select_implemented_bit_5 ? "RATE_SELECT functionality implemented" : "RATE_SELECT functionality NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 4]", programming.byte_65_option_values.tx_disable_implemented_bit_4 ? "TX_DISABLE implemented" : "TX_DISABLE NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 3]", programming.byte_65_option_values.tx_fault_implemented_bit_3 ? "TX_FAULT signal implemented" : "TX_FAULT signal NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 2]", programming.byte_65_option_values.loss_of_signal_inverted_bit_2 ? "Loss of Signal with inverted signal implemented" : "Loss of Signal with inverted signal NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 1]", programming.byte_65_option_values.loss_of_signal_implemented_bit_1 ? "Loss of Signal implemented" : "Loss of Signal NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [65, 0]", formatReservedBit(programming.byte_65_option_values.reserved_bit_0)
    );
    str.append("\n");


    if(programming.byte_12_nominal_signaling_rate_in_100_mbaud != 0xFF) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString,
            "Signaling Rate, max [66]",
            fmt::format(
                "{}% Margin ({}% above {} MBd -> Max {} MBd)",
                programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud,
                programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud,
                (int)(programming.byte_12_nominal_signaling_rate_in_100_mbaud) * 100,
                (int)(programming.byte_12_nominal_signaling_rate_in_100_mbaud) * (100 + programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud)
            )
        );
        str.append("\n");
        fmt::format_to(std::back_inserter(str), optionTitleFormatString,
            "Signaling Rate, min [67]",
            fmt::format(
                "{}% Margin ({}% below {} MBd -> Min {} MBd)",
                programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent,
                programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent,
                (int)(programming.byte_12_nominal_signaling_rate_in_100_mbaud) * 100,
                (int)(programming.byte_12_nominal_signaling_rate_in_100_mbaud) * (100 - programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent)
            )
        );
        str.append("\n");
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Signaling Rate, nominal [66]", programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud != 0x00 ? fmt::format("{} MBd", int(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250) : "Unspecified"
        );
        str.append("\n");
        fmt::format_to(std::back_inserter(str), optionTitleFormatString,
            "Signaling Rate, Range [67]",
            fmt::format(
                "+/-{}% Margin (+/-{}% around {} MBd -> Min {} Mbd, Max {} Mbd)",
                programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent,
                programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent,
                (int)(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250,
                (double)(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250.0 * (1.0 - (double)(programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent) / 100.0),
                (double)(programming.byte_66_max_signaling_rate_in_percent_or_nominal_signaling_rate_in_250_mbaud) * 250.0 * (1.0 + (double)(programming.byte_67_min_signaling_rate_in_percent_or_range_of_signaling_rates_in_percent) / 100.0)
            )
        );
        str.append("\n");
    }
    
    bool vendorSerialNumberPrintable = std::all_of(
        programming.byte_68_83_vendor_sn.begin(),
        programming.byte_68_83_vendor_sn.end(),
        [](char c) {return std::isprint(c); }
    );
    if(vendorSerialNumberPrintable) {
        std::string vendorSerialNumber = std::string(reinterpret_cast<char const *>(programming.byte_68_83_vendor_sn.data()), 16);
        //rtrim
        vendorSerialNumber.erase(std::find_if(vendorSerialNumber.rbegin(), vendorSerialNumber.rend(), [](unsigned char ch) { return !(ch == 0x20); }).base(), vendorSerialNumber.end());

        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Vendor S/N (wrapping quotes added by TransceiverTool) [68-83]", vendorSerialNumber
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Vendor S/N (contains unprintable characters, printed as hex bytes) [68-83]"
        );

        for(int index = 0; index < programming.byte_68_83_vendor_sn.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_68_83_vendor_sn[index]);
        }
        str.append("\n");
    }
    str.append("\n");

    bool dateCodeYearPrintable = std::all_of(
        programming.byte_84_91_date_code.year_low_order_digits.begin(),
        programming.byte_84_91_date_code.year_low_order_digits.end(),
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeYearPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, low order digits of year [84-85]", std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.year_low_order_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, low order digits of year (invalid format, printed as hex bytes) [84-85]"
        );

        for(int index = 0; index < programming.byte_84_91_date_code.year_low_order_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_84_91_date_code.year_low_order_digits[index]);
        }
        str.append("\n");
    }

    bool dateCodeMonthPrintable = std::all_of(
        programming.byte_84_91_date_code.month_digits.begin(),
        programming.byte_84_91_date_code.month_digits.end(), 
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeMonthPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, digits of month [86-87]", std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.month_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, digits of month (invalid format, printed as hex bytes) [86-87]"
        );

        for(int index = 0; index < programming.byte_84_91_date_code.month_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_84_91_date_code.month_digits[index]);
        }
        str.append("\n");
    }

    bool dateCodeDayPrintable = std::all_of(
        programming.byte_84_91_date_code.day_digits.begin(),
        programming.byte_84_91_date_code.day_digits.end(),
        [](char c) {return std::isdigit(c); }
    );
    if(dateCodeDayPrintable) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "Date Code, digits of day [88-89]", std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.day_digits.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, digits of day (invalid format, printed as hex bytes) [88-89]"
        );

        for(int index = 0; index < programming.byte_84_91_date_code.day_digits.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_84_91_date_code.day_digits[index]);
        }
        str.append("\n");
    }

    bool lotCodePrintable = std::all_of(programming.byte_84_91_date_code.lot_code.begin(), programming.byte_84_91_date_code.lot_code.end(), [](char c) {return std::isprint(c); });
    if(lotCodePrintable) {
        fmt::format_to(std::back_inserter(str), "{: <85s}: {:?}\n", 
            "Date Code, lot code (wrapping quotes added by TransceiverTool) [90-91]", std::string(reinterpret_cast<char const *>(programming.byte_84_91_date_code.lot_code.data()), 2)
        );
    } else {
        fmt::format_to(std::back_inserter(str), "{: <85s}:", 
            "Date Code, lot code (invalid format, printed as hex bytes) [90-91]"
        );

        for(int index = 0; index < programming.byte_84_91_date_code.lot_code.size(); ++index) {
            fmt::format_to(std::back_inserter(str), " {:#04x}", programming.byte_84_91_date_code.lot_code[index]);
        }
        str.append("\n");
    }
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 7]", formatReservedBit(programming.byte_92_diagnostic_monitoring_type.reserved_bit_7)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 6]", programming.byte_92_diagnostic_monitoring_type.digital_diagnostic_monitoring_implemented_bit_6 ? "Digital diagnostic monitoring implemented" : "Digital diagnostic monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 5]", programming.byte_92_diagnostic_monitoring_type.internally_calibrated_bit_5 ? "Internally Calibrated" : "NOT Internally Calibrated"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 4]", programming.byte_92_diagnostic_monitoring_type.externally_calibrated_bit_4 ? "Externally Calibrated" : "NOT Externally Calibrated"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 3]", programming.byte_92_diagnostic_monitoring_type.received_power_measurement_is_average_bit_3 ? "Received power measurements type = Average" : "Received power measurements type = OMA"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 2]", programming.byte_92_diagnostic_monitoring_type.address_change_required_bit_2 ? "Address change required" : "Address change NOT required"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 1]", formatReservedBit(programming.byte_92_diagnostic_monitoring_type.reserved_bit_1)
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Option Values [92, 0]", formatReservedBit(programming.byte_92_diagnostic_monitoring_type.reserved_bit_0)
    );
    str.append("\n");


    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 7]", programming.byte_93_enhanced_options.optional_alarm_warning_flags_implemented_bit_7 ? "Optional Alarm/warning flags implemented" : "Optional Alarm/warning flags NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 6]", programming.byte_93_enhanced_options.optional_soft_TX_DISABLE_control_and_monitoring_implemented_bit_6 ? "Optional soft TX_DISABLE control and monitoring implemented" : "Optional soft TX_DISABLE control and monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 5]", programming.byte_93_enhanced_options.optional_soft_TX_FAULT_monitoring_implemented_bit_5 ? "Optional soft TX_FAULT monitoring implemented" : "Optional soft TX_FAULT monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 4]", programming.byte_93_enhanced_options.optional_soft_RX_LOS_monitoring_implemented_bit_4 ? "Optional soft RX_LOS monitoring implemented" : "Optional soft RX_LOS monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 3]", programming.byte_93_enhanced_options.optional_soft_RATE_SELECT_control_and_monitoring_implemented_bit_3 ? "Optional soft RATE_SELECT control and monitoring implemented" : "Optional soft RATE_SELECT control and monitoring NOT implemented"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 2]", programming.byte_93_enhanced_options.optional_application_select_control_implemented_per_SFF_8079_bit_2 ? "Optional Application Select control implemented per SFF-8079" : "Optional Application Select control NOT implemented per SFF-8079"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 1]", programming.byte_93_enhanced_options.optional_soft_rate_select_control_implemented_per_SFF_8431_bit_1 ? "Optional soft Rate Select control implemented per SFF-8431" : "Optional soft Rate Select control NOT implemented per SFF-8431"
    );
    fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
        "Enhanced Options [93, 0]", formatReservedBit(programming.byte_93_enhanced_options.reserved_bit_0)
    );
    str.append("\n");



    if(programming.byte_95_CC_EXT == CC_EXT) {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_EXT checksum [95]", fmt::format("{:#04x} (correct)", programming.byte_95_CC_EXT)
        );
    } else {
        fmt::format_to(std::back_inserter(str), optionTitleFormatString, 
            "CC_EXT checksum [95]", fmt::format("{:#04x} (incorrect, should be {:#04x})", programming.byte_95_CC_EXT, CC_EXT)
        );
    }
    str.append("\n");

    return str;

}