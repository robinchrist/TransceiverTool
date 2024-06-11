#include "TransceiverTool/Standards/SFF-8636_Parser.hpp"
#include <cstring>

namespace TransceiverTool::Standards::SFF8636 {

    //Expects 256 bytes!
    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h parseBytesToStruct(unsigned char const * bytes) {
        SFF8636_Upper00h parsedStruct;

        parsedStruct.byte_128_Identifier = bytes[128];

        parsedStruct.byte_129_extended_identifier.power_class_bit_7_6 = getSFF8636_Extended_Identifier_Bit_7_6_From_Char(bytes[129]);
        parsedStruct.byte_129_extended_identifier.power_class_8_bit_5 = bytes[129] & (1 << 5);
        parsedStruct.byte_129_extended_identifier.clei_code_bit_4 = bytes[129] & (1 << 4);
        parsedStruct.byte_129_extended_identifier.cdr_in_tx_bit_3 = bytes[129] & (1 << 3);
        parsedStruct.byte_129_extended_identifier.cdr_in_rx_bit_2 = bytes[129] & (1 << 2);
        parsedStruct.byte_129_extended_identifier.power_class_bit_1_0 = getSFF8636_Extended_Identifier_Bit_1_0_From_Char(bytes[129]);

        parsedStruct.byte_130_Connector_Type = bytes[130];

        parsedStruct.byte_131_ethernet_compliance_codes.extended_bit_7 = bytes[131] & (1 << 7);
        parsedStruct.byte_131_ethernet_compliance_codes._10GBASE_LRM_bit_6 = bytes[131] & (1 << 6);
        parsedStruct.byte_131_ethernet_compliance_codes._10GBASE_LR_bit_5 = bytes[131] & (1 << 5);
        parsedStruct.byte_131_ethernet_compliance_codes._10GBASE_SR_bit_4 = bytes[131] & (1 << 4);
        parsedStruct.byte_131_ethernet_compliance_codes._40GBASE_CR4_bit_3 = bytes[131] & (1 << 3);
        parsedStruct.byte_131_ethernet_compliance_codes._40GBASE_SR4_bit_2 = bytes[131] & (1 << 2);
        parsedStruct.byte_131_ethernet_compliance_codes._40GBASE_LR4_bit_1 = bytes[131] & (1 << 1);
        parsedStruct.byte_131_ethernet_compliance_codes._40G_Active_Cable_XLPPI_bit_0 = bytes[131] & (1 << 0);

        parsedStruct.byte_132_sonet_compliance_codes.reserved_bit_7 = bytes[132] & (1 << 7);
        parsedStruct.byte_132_sonet_compliance_codes.reserved_bit_6 = bytes[132] & (1 << 6);
        parsedStruct.byte_132_sonet_compliance_codes.reserved_bit_5 = bytes[132] & (1 << 5);
        parsedStruct.byte_132_sonet_compliance_codes.reserved_bit_4 = bytes[132] & (1 << 4);
        parsedStruct.byte_132_sonet_compliance_codes.reserved_bit_3 = bytes[132] & (1 << 3);
        parsedStruct.byte_132_sonet_compliance_codes.OC48_long_reach_bit_2 = bytes[132] & (1 << 2);
        parsedStruct.byte_132_sonet_compliance_codes.OC48_intermediate_reach_bit_1 = bytes[132] & (1 << 1);
        parsedStruct.byte_132_sonet_compliance_codes.OC48_short_reach_bit_0 = bytes[132] & (1 << 0);

        parsedStruct.byte_133_sas_sata_compliance_codes.SAS_24_0_Gbps_bit_7 = bytes[133] & (1 << 7);
        parsedStruct.byte_133_sas_sata_compliance_codes.SAS_12_0_Gbps_bit_6 = bytes[133] & (1 << 6);
        parsedStruct.byte_133_sas_sata_compliance_codes.SAS_6_0_Gbps_bit_5 = bytes[133] & (1 << 5);
        parsedStruct.byte_133_sas_sata_compliance_codes.SAS_3_0_Gbps_bit_4 = bytes[133] & (1 << 4);
        parsedStruct.byte_133_sas_sata_compliance_codes.reserved_bit_3 = bytes[133] & (1 << 3);
        parsedStruct.byte_133_sas_sata_compliance_codes.reserved_bit_2 = bytes[133] & (1 << 2);
        parsedStruct.byte_133_sas_sata_compliance_codes.reserved_bit_1 = bytes[133] & (1 << 1);
        parsedStruct.byte_133_sas_sata_compliance_codes.reserved_bit_0 = bytes[133] & (1 << 0);

        parsedStruct.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7 = bytes[134] & (1 << 7);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6 = bytes[134] & (1 << 6);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5 = bytes[134] & (1 << 5);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4 = bytes[134] & (1 << 4);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes._1000BASE_T_bit_3 = bytes[134] & (1 << 3);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes._1000BASE_CX_bit_2 = bytes[134] & (1 << 2);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes._1000BASE_LX_bit_1 = bytes[134] & (1 << 1);
        parsedStruct.byte_134_gigabit_ethernet_compliance_codes._1000BASE_SX_bit_0 = bytes[134] & (1 << 0);

        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Very_long_distance_V_bit_7 = bytes[135] & (1 << 7);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Short_distance_S_bit_6 = bytes[135] & (1 << 6);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Intermediate_distance_I_bit_5 = bytes[135] & (1 << 5);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Long_distance_L_bit_4 = bytes[135] & (1 << 4);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Medium_M_bit_3 = bytes[135] & (1 << 3);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.reserved_Transmitter_Technology_bit_2 = bytes[135] & (1 << 2);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Longwave_laser_LC_bit_1 = bytes[135] & (1 << 1);
        parsedStruct.byte_135_fibre_channel_link_length_and_transmitter_technology.Electrical_inter_enclosure_EL_bit_0 = bytes[135] & (1 << 0);

        parsedStruct.byte_136_fibre_channel_transmitter_technology.Electrical_intra_enclosure_bit_7 = bytes[136] & (1 << 7);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_o_OFC_SN_bit_6 = bytes[136] & (1 << 6);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_OFC_SL_bit_5 = bytes[136] & (1 << 5);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.Longwave_Laser_LL_bit_4 = bytes[136] & (1 << 4);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.reserved_bit_3 = bytes[136] & (1 << 3);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.reserved_bit_2 = bytes[136] & (1 << 2);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.reserved_bit_1 = bytes[136] & (1 << 1);
        parsedStruct.byte_136_fibre_channel_transmitter_technology.reserved_bit_0 = bytes[136] & (1 << 0);

        parsedStruct.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7 = bytes[137] & (1 << 7);
        parsedStruct.byte_137_fibre_channel_transmission_media.Shielded_Twisted_Pair_TP_bit_6 = bytes[137] & (1 << 6);
        parsedStruct.byte_137_fibre_channel_transmission_media.Miniature_Coax_MI_bit_5 = bytes[137] & (1 << 5);
        parsedStruct.byte_137_fibre_channel_transmission_media.Video_Coax_TV_bit_4 = bytes[137] & (1 << 4);
        parsedStruct.byte_137_fibre_channel_transmission_media.Multi_mode_62_5_um_M6_bit_3 = bytes[137] & (1 << 3);
        parsedStruct.byte_137_fibre_channel_transmission_media.Multi_mode_50_um_M5_bit_2 = bytes[137] & (1 << 2);
        parsedStruct.byte_137_fibre_channel_transmission_media.Multi_mode_50_um_OM3_bit_1 = bytes[137] & (1 << 1);
        parsedStruct.byte_137_fibre_channel_transmission_media.Single_Mode_SM_bit_0 = bytes[137] & (1 << 0);

        parsedStruct.byte_138_fibre_channel_speed._1200_MBps_per_channel_bit_7 = bytes[138] & (1 << 7);
        parsedStruct.byte_138_fibre_channel_speed._800_MBps_bit_6 = bytes[138] & (1 << 6);
        parsedStruct.byte_138_fibre_channel_speed._1600_MBps_per_channel_bit_5 = bytes[138] & (1 << 5);
        parsedStruct.byte_138_fibre_channel_speed._400_MBps_bit_4 = bytes[138] & (1 << 4);
        parsedStruct.byte_138_fibre_channel_speed._3200_MBps_per_channel_bit_3 = bytes[138] & (1 << 3);
        parsedStruct.byte_138_fibre_channel_speed._200_MBps_bit_2 = bytes[138] & (1 << 2);
        parsedStruct.byte_138_fibre_channel_speed.Extended_bit_1 = bytes[138] & (1 << 1);
        parsedStruct.byte_138_fibre_channel_speed._100_MBps_bit_0 = bytes[138] & (1 << 0);

        parsedStruct.byte_139_Encoding = bytes[139];

        parsedStruct.byte_140_nominal_signaling_rate_in_100_mbaud = bytes[140];

        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_7 = bytes[141] & (1 << 7);
        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_6 = bytes[141] & (1 << 6);
        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_5 = bytes[141] & (1 << 5);
        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_4 = bytes[141] & (1 << 4);
        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_3 = bytes[141] & (1 << 3);
        parsedStruct.byte_141_extended_rate_select_compliance.reserved_bit_2 = bytes[141] & (1 << 2);
        parsedStruct.byte_141_extended_rate_select_compliance.rate_select_bits_1_0 = getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0_From_Char(bytes[141]);

        parsedStruct.byte_142_length_smf_in_kilometers = bytes[142];

        parsedStruct.byte_143_length_om3_in_2m = bytes[143];

        parsedStruct.byte_144_length_om2_in_m = bytes[144];

        parsedStruct.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB = bytes[145];

        parsedStruct.byte_146_length_copper_in_1m_or_om4_in_2m = bytes[146];

        parsedStruct.byte_147_device_technology_and_transmitter_technology.transmitter_Technology_bit_7_4 = getSFF8636_Transmitter_Technology_bit_7_4_From_Char(bytes[147]);
        parsedStruct.byte_147_device_technology_and_transmitter_technology.wavelength_control_bit_3 = bytes[147] & (1 << 3);
        parsedStruct.byte_147_device_technology_and_transmitter_technology.cooled_transmitter_bit_2 = bytes[147] & (1 << 2);
        parsedStruct.byte_147_device_technology_and_transmitter_technology.pin_apd_detector_bit_1 = bytes[147] & (1 << 1);
        parsedStruct.byte_147_device_technology_and_transmitter_technology.transmitter_tunable_bit_0 = bytes[147] & (1 << 0);

        std::memcpy(parsedStruct.byte_148_163_vendor_name.data(), bytes + 148, 16);

        parsedStruct.byte_164_extended_module_codes.reserved_bit_7 = bytes[164] & (1 << 7);
        parsedStruct.byte_164_extended_module_codes.reserved_bit_6 = bytes[164] & (1 << 6);
        parsedStruct.byte_164_extended_module_codes.HDR_bit_5 = bytes[164] & (1 << 5);
        parsedStruct.byte_164_extended_module_codes.EDR_bit_4 = bytes[164] & (1 << 4);
        parsedStruct.byte_164_extended_module_codes.FDR_bit_3 = bytes[164] & (1 << 3);
        parsedStruct.byte_164_extended_module_codes.QDR_bit_2 = bytes[164] & (1 << 2);
        parsedStruct.byte_164_extended_module_codes.DDR_bit_1 = bytes[164] & (1 << 1);
        parsedStruct.byte_164_extended_module_codes.SDR_bit_0 = bytes[164] & (1 << 0);

        std::memcpy(parsedStruct.byte_165_167_vendor_oui.data(), bytes + 165, 3);

        std::memcpy(parsedStruct.byte_168_183_vendor_pn.data(), bytes + 168, 16);

        std::memcpy(parsedStruct.byte_184_185_vendor_rev.data(), bytes + 184, 2);

        parsedStruct.byte_186_wavelength_high_order_or_copper_attenuation = bytes[186];
        parsedStruct.byte_187_wavelength_low_order_or_copper_attenuation = bytes[187];

        parsedStruct.byte_188_wavelength_tolerance_high_order_or_copper_attenuation = bytes[188];
        parsedStruct.byte_189_wavelength_tolerance_low_order_or_copper_attenuation = bytes[189];

        parsedStruct.byte_190_max_case_temperature = bytes[190];

        parsedStruct.byte_191_CC_BASE = bytes[191];

        parsedStruct.byte_192_extended_specification_compliance_codes = bytes[192];

        parsedStruct.byte_193_option_values.reserved_bit_7 = bytes[193] & (1 << 7);
        parsedStruct.byte_193_option_values.lpmode_txdis_input_configurable_bit_6 = bytes[193] & (1 << 6);
        parsedStruct.byte_193_option_values.intl_rxlosl_output_configurable_bit_5 = bytes[193] & (1 << 5);
        parsedStruct.byte_193_option_values.tx_input_adaptive_equalizers_freeze_capable_bit_4 = bytes[193] & (1 << 4);
        parsedStruct.byte_193_option_values.tx_input_equalizers_auto_adaptive_capable_bit_3 = bytes[193] & (1 << 3);
        parsedStruct.byte_193_option_values.tx_input_equalizers_fixed_programmable_settings_bit_2 = bytes[193] & (1 << 2);
        parsedStruct.byte_193_option_values.rx_output_emphasis_fixed_programmable_settings_bit_1 = bytes[193] & (1 << 1);
        parsedStruct.byte_193_option_values.rx_output_amplitude_fixed_programmable_settings_bit_0 = bytes[193] & (1 << 0);

        parsedStruct.byte_194_option_values.tx_cdr_on_off_control_implemented_bit_7 = bytes[194] & (1 << 7);
        parsedStruct.byte_194_option_values.rx_cdr_on_off_control_implemented_bit_6 = bytes[194] & (1 << 6);
        parsedStruct.byte_194_option_values.tx_cdr_loss_of_lock_lol_flag_implemented_bit_5 = bytes[194] & (1 << 5);
        parsedStruct.byte_194_option_values.rx_cdr_loss_of_lock_lol_flag_implemented_bit_4 = bytes[194] & (1 << 4);
        parsedStruct.byte_194_option_values.rx_squelch_disable_implemented_bit_3 = bytes[194] & (1 << 3);
        parsedStruct.byte_194_option_values.rx_output_disable_implemented_bit_2 = bytes[194] & (1 << 2);
        parsedStruct.byte_194_option_values.tx_squelch_disable_implemented_bit_1 = bytes[194] & (1 << 1);
        parsedStruct.byte_194_option_values.tx_squelch_implemented_bit_0 = bytes[194] & (1 << 0);

        parsedStruct.byte_195_option_values.memory_page_02_provided_bit_7 = bytes[195] & (1 << 7);
        parsedStruct.byte_195_option_values.memory_page_01h_provided_bit_6 = bytes[195] & (1 << 6);
        parsedStruct.byte_195_option_values.rate_select_implemented_bit_5 = bytes[195] & (1 << 5);
        parsedStruct.byte_195_option_values.tx_disable_implemented_bit_4 = bytes[195] & (1 << 4);
        parsedStruct.byte_195_option_values.tx_fault_signal_implemented_bit_3 = bytes[195] & (1 << 3);
        parsedStruct.byte_195_option_values.tx_squelch_implemented_to_reduce_pave_bit_2 = bytes[195] & (1 << 2);
        parsedStruct.byte_195_option_values.tx_loss_of_signal_implemented_bit_1 = bytes[195] & (1 << 1);
        parsedStruct.byte_195_option_values.pages_20_to_21h_implemented_bit_0 = bytes[195] & (1 << 0);

        std::memcpy(parsedStruct.byte_196_211_vendor_sn.data(), bytes + 196, 16);

        std::memcpy(parsedStruct.byte_212_219_date_code.year_low_order_digits.data(), bytes + 212, 2);
        std::memcpy(parsedStruct.byte_212_219_date_code.month_digits.data(), bytes + 214, 2);
        std::memcpy(parsedStruct.byte_212_219_date_code.day_digits.data(), bytes + 216, 2);
        std::memcpy(parsedStruct.byte_212_219_date_code.lot_code.data(), bytes + 218, 2);

        parsedStruct.byte_220_diagnostic_monitoring_type.reserved_bit_7 = bytes[220] & (1 << 7);
        parsedStruct.byte_220_diagnostic_monitoring_type.reserved_bit_6 = bytes[220] & (1 << 6);
        parsedStruct.byte_220_diagnostic_monitoring_type.temperature_monitoring_implemented_bit_5 = bytes[220] & (1 << 5);
        parsedStruct.byte_220_diagnostic_monitoring_type.supply_voltage_monitoring_implemented_bit_4 = bytes[220] & (1 << 4);
        parsedStruct.byte_220_diagnostic_monitoring_type.received_power_measurement_is_average_bit_3 = bytes[220] & (1 << 3);
        parsedStruct.byte_220_diagnostic_monitoring_type.transmitter_power_measurement_supported_bit_2 = bytes[220] & (1 << 2);
        parsedStruct.byte_220_diagnostic_monitoring_type.reserved_bit_1 = bytes[220] & (1 << 1);
        parsedStruct.byte_220_diagnostic_monitoring_type.reserved_bit_0 = bytes[220] & (1 << 0);

        parsedStruct.byte_221_enhanced_options.reserved_bit_7 = bytes[221] & (1 << 7);
        parsedStruct.byte_221_enhanced_options.reserved_bit_6 = bytes[221] & (1 << 6);
        parsedStruct.byte_221_enhanced_options.reserved_bit_5 = bytes[221] & (1 << 5);
        parsedStruct.byte_221_enhanced_options.initialization_complete_flag_implemented_bit_4 = bytes[221] & (1 << 4);
        parsedStruct.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3 = bytes[221] & (1 << 3);
        parsedStruct.byte_221_enhanced_options.reserved_bit_2 = bytes[221] & (1 << 2);
        parsedStruct.byte_221_enhanced_options.readiness_flag_implemented_bit_1 = bytes[221] & (1 << 1);
        parsedStruct.byte_221_enhanced_options.software_reset_implemented_bit_0 = bytes[221] & (1 << 0);

        parsedStruct.byte_222_extended_baud_rate_in_250_mbaud = bytes[222];
        
        parsedStruct.byte_223_CC_EXT = bytes[223];

        std::memcpy(parsedStruct.byte_224_255_vendor_specific.data(), bytes + 224, 32);

        return parsedStruct;
    }

}