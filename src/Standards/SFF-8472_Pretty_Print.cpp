#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8472_Compliance_Codes.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Extended_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8472_Physical_Device_Identifier_Values.hpp"
#include "TransceiverTool/Standards/SFF-8024_Transceiver_Connector_Type.hpp"
#include "fmt/format.h"
#include "fmt/color.h"
#include <algorithm>
#include <iterator>
#include "TransceiverTool/Vendor_OUIs.hpp"


std::string TransceiverTool::Standards::SFF8472::prettyPrintProgramming(const SFF8472_LowerA0h &programming, bool fiberMode, bool copperMode) {

    std::string optionTitleFormatString = "{: <85s}: {:s}\n";
    auto formatReservedBit = [](bool bit) -> std::string {
        if(bit) {
            return fmt::format("{}", fmt::styled("Reserved bit set", fmt::bg(fmt::color::red)));
        } else {
            return "N/A";
        }
    };


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

    return str;

}