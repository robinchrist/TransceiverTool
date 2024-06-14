#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_Checksum.hpp"
#include <stdexcept>

namespace TransceiverTool::Standards::SFF8636 {


    //Creates the last 128 bytes of target (page 00h SFF-8636)
    //i.e. target must be 256 bytes big
    void assembleToBinary(unsigned char * target, const SFF8636_Upper00h& programming, Standards::common::ChecksumDirective CC_BASEDirective, Standards::common::ChecksumDirective CC_EXTDirective) {

        target[128] = programming.byte_128_Identifier;

        target[129] = 
            getSFF8636_Extended_Identifier_Bit_7_6Info(programming.byte_129_extended_identifier.power_class_bit_7_6).bitValue << 6 |
            (unsigned char)(programming.byte_129_extended_identifier.power_class_8_bit_5) << 5 |
            (unsigned char)(programming.byte_129_extended_identifier.clei_code_bit_4) << 4 |
            (unsigned char)(programming.byte_129_extended_identifier.cdr_in_tx_bit_3) << 3 |
            (unsigned char)(programming.byte_129_extended_identifier.cdr_in_rx_bit_2) << 2 |
            getSFF8636_Extended_Identifier_Bit_1_0Info(programming.byte_129_extended_identifier.power_class_bit_1_0).bitValue;
        

        target[130] = programming.byte_130_Connector_Type;

        target[131] = 
            (unsigned char)(programming.byte_131_ethernet_compliance_codes.extended_bit_7) << 7 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._10GBASE_LRM_bit_6) << 6 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._10GBASE_LR_bit_5) << 5 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._10GBASE_SR_bit_4) << 4 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._40GBASE_CR4_bit_3) << 3 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._40GBASE_SR4_bit_2) << 2 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._40GBASE_LR4_bit_1) << 1 |
            (unsigned char)(programming.byte_131_ethernet_compliance_codes._40G_Active_Cable_XLPPI_bit_0) << 0;

        target[132] = 
            (unsigned char)(programming.byte_132_sonet_compliance_codes.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.reserved_bit_5) << 5 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.reserved_bit_4) << 4 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.reserved_bit_3) << 3 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.OC48_long_reach_bit_2) << 2 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.OC48_intermediate_reach_bit_1) << 1 |
            (unsigned char)(programming.byte_132_sonet_compliance_codes.OC48_short_reach_bit_0) << 0;

        target[133] = 
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.SAS_24_0_Gbps_bit_7) << 7 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.SAS_12_0_Gbps_bit_6) << 6 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.SAS_6_0_Gbps_bit_5) << 5 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.SAS_3_0_Gbps_bit_4) << 4 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.reserved_bit_3) << 3 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.reserved_bit_2) << 2 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_133_sas_sata_compliance_codes.reserved_bit_0) << 0;

        target[134] = 
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_5) << 5 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes.reserved_bit_4) << 4 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_T_bit_3) << 3 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_CX_bit_2) << 2 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_LX_bit_1) << 1 |
            (unsigned char)(programming.byte_134_gigabit_ethernet_compliance_codes._1000BASE_SX_bit_0) << 0;

        target[135] = 
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Very_long_distance_V_bit_7) << 7 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Short_distance_S_bit_6) << 6 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Intermediate_distance_I_bit_5) << 5 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Long_distance_L_bit_4) << 4 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Medium_M_bit_3) << 3 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.reserved_Transmitter_Technology_bit_2) << 2 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Longwave_laser_LC_bit_1) << 1 |
            (unsigned char)(programming.byte_135_fibre_channel_link_length_and_transmitter_technology.Electrical_inter_enclosure_EL_bit_0) << 0;

        target[136] = 
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.Electrical_intra_enclosure_bit_7) << 7 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_o_OFC_SN_bit_6) << 6 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.Shortwave_laser_w_OFC_SL_bit_5) << 5 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.Longwave_Laser_LL_bit_4) << 4 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_3) << 3 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_2) << 2 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_136_fibre_channel_transmitter_technology.reserved_bit_0) << 0;

        target[137] = 
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Twin_Axial_Pair_TW_bit_7) << 7 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Shielded_Twisted_Pair_TP_bit_6) << 6 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Miniature_Coax_MI_bit_5) << 5 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Video_Coax_TV_bit_4) << 4 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Multi_mode_62_5_um_M6_bit_3) << 3 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Multi_mode_50_um_M5_bit_2) << 2 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Multi_mode_50_um_OM3_bit_1) << 1 |
            (unsigned char)(programming.byte_137_fibre_channel_transmission_media.Single_Mode_SM_bit_0) << 0;

        target[138] = 
            (unsigned char)(programming.byte_138_fibre_channel_speed._1200_MBps_per_channel_bit_7) << 7 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._800_MBps_bit_6) << 6 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._1600_MBps_per_channel_bit_5) << 5 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._400_MBps_bit_4) << 4 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._3200_MBps_per_channel_bit_3) << 3 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._200_MBps_bit_2) << 2 |
            (unsigned char)(programming.byte_138_fibre_channel_speed.Extended_bit_1) << 1 |
            (unsigned char)(programming.byte_138_fibre_channel_speed._100_MBps_bit_0) << 0;

        target[139] = programming.byte_139_Encoding;

        target[140] = programming.byte_140_nominal_signaling_rate_in_100_mbaud;

        target[141] = 
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_5) << 5 |
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_4) << 4 |
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_3) << 3 |
            (unsigned char)(programming.byte_141_extended_rate_select_compliance.reserved_bit_2) << 2 |
            getSFF8636_Extended_Rate_Select_Compliance_Bit_1_0Info(programming.byte_141_extended_rate_select_compliance.rate_select_bits_1_0).bitValue << 0;

        target[142] = programming.byte_142_length_smf_in_kilometers;

        target[143] = programming.byte_143_length_om3_in_2m;

        target[144] = programming.byte_144_length_om2_in_m;

        target[145] = programming.byte_145_length_om1_in_1m_or_copper_cable_attenuation_in_dB;

        target[146] = programming.byte_146_length_copper_in_1m_or_om4_in_2m;

        target[147] = 
            getSFF8636_Transmitter_Technology_bit_7_4Info(programming.byte_147_device_technology_and_transmitter_technology.transmitter_Technology_bit_7_4).bitValue << 4 |
            (unsigned char)(programming.byte_147_device_technology_and_transmitter_technology.wavelength_control_bit_3) << 3 |
            (unsigned char)(programming.byte_147_device_technology_and_transmitter_technology.cooled_transmitter_bit_2) << 2 |
            (unsigned char)(programming.byte_147_device_technology_and_transmitter_technology.pin_apd_detector_bit_1) << 1 |
            (unsigned char)(programming.byte_147_device_technology_and_transmitter_technology.transmitter_tunable_bit_0) << 0;

        std::memcpy(target + 148, programming.byte_148_163_vendor_name.data(), 16);

        target[164] = 
            (unsigned char)(programming.byte_164_extended_module_codes.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_164_extended_module_codes.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_164_extended_module_codes.HDR_bit_5) << 5 |
            (unsigned char)(programming.byte_164_extended_module_codes.EDR_bit_4) << 4 |
            (unsigned char)(programming.byte_164_extended_module_codes.FDR_bit_3) << 3 |
            (unsigned char)(programming.byte_164_extended_module_codes.QDR_bit_2) << 2 |
            (unsigned char)(programming.byte_164_extended_module_codes.DDR_bit_1) << 1 |
            (unsigned char)(programming.byte_164_extended_module_codes.SDR_bit_0) << 0;

        std::memcpy(target + 165, programming.byte_165_167_vendor_oui.data(), 3);

        std::memcpy(target + 168, programming.byte_168_183_vendor_pn.data(), 16);

        std::memcpy(target + 184, programming.byte_184_185_vendor_rev.data(), 2);

        target[186] = programming.byte_186_wavelength_high_order_or_copper_attenuation;
        target[187] = programming.byte_187_wavelength_low_order_or_copper_attenuation;

        target[188] = programming.byte_188_wavelength_tolerance_high_order_or_copper_attenuation;
        target[189] = programming.byte_189_wavelength_tolerance_low_order_or_copper_attenuation;

        target[190] = programming.byte_190_max_case_temperature;

        if(CC_BASEDirective == Standards::common::ChecksumDirective::AUTO_CALCULATE_FROM_CONTENT) {
            target[191] = calculateCC_BASEChecksum(target);
        } else if(CC_BASEDirective == Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING) {
            target[191] = programming.byte_191_CC_BASE;
        } else {
            throw std::runtime_error("Unhandled enum case!");
        }

        target[192] = programming.byte_192_extended_specification_compliance_codes;

        target[193] =
            (unsigned char)(programming.byte_193_option_values.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_193_option_values.lpmode_txdis_input_configurable_bit_6) << 6 |
            (unsigned char)(programming.byte_193_option_values.intl_rxlosl_output_configurable_bit_5) << 5 |
            (unsigned char)(programming.byte_193_option_values.tx_input_adaptive_equalizers_freeze_capable_bit_4) << 4 |
            (unsigned char)(programming.byte_193_option_values.tx_input_equalizers_auto_adaptive_capable_bit_3) << 3 |
            (unsigned char)(programming.byte_193_option_values.tx_input_equalizers_fixed_programmable_settings_bit_2) << 2 |
            (unsigned char)(programming.byte_193_option_values.rx_output_emphasis_fixed_programmable_settings_bit_1) << 1 |
            (unsigned char)(programming.byte_193_option_values.rx_output_amplitude_fixed_programmable_settings_bit_0) << 0;

        target[194] =
            (unsigned char)(programming.byte_194_option_values.tx_cdr_on_off_control_implemented_bit_7) << 7 |
            (unsigned char)(programming.byte_194_option_values.rx_cdr_on_off_control_implemented_bit_6) << 6 |
            (unsigned char)(programming.byte_194_option_values.tx_cdr_loss_of_lock_lol_flag_implemented_bit_5) << 5 |
            (unsigned char)(programming.byte_194_option_values.rx_cdr_loss_of_lock_lol_flag_implemented_bit_4) << 4 |
            (unsigned char)(programming.byte_194_option_values.rx_squelch_disable_implemented_bit_3) << 3 |
            (unsigned char)(programming.byte_194_option_values.rx_output_disable_implemented_bit_2) << 2 |
            (unsigned char)(programming.byte_194_option_values.tx_squelch_disable_implemented_bit_1) << 1 |
            (unsigned char)(programming.byte_194_option_values.tx_squelch_implemented_bit_0) << 0;

        target[195] =
            (unsigned char)(programming.byte_195_option_values.memory_page_02_provided_bit_7) << 7 |
            (unsigned char)(programming.byte_195_option_values.memory_page_01h_provided_bit_6) << 6 |
            (unsigned char)(programming.byte_195_option_values.rate_select_implemented_bit_5) << 5 |
            (unsigned char)(programming.byte_195_option_values.tx_disable_implemented_bit_4) << 4 |
            (unsigned char)(programming.byte_195_option_values.tx_fault_signal_implemented_bit_3) << 3 |
            (unsigned char)(programming.byte_195_option_values.tx_squelch_implemented_to_reduce_pave_bit_2) << 2 |
            (unsigned char)(programming.byte_195_option_values.tx_loss_of_signal_implemented_bit_1) << 1 |
            (unsigned char)(programming.byte_195_option_values.pages_20_to_21h_implemented_bit_0) << 0;

        std::memcpy(target + 196, programming.byte_196_211_vendor_sn.data(), 16);

        std::memcpy(target + 212, programming.byte_212_219_date_code.year_low_order_digits.data(), 2);
        std::memcpy(target + 214, programming.byte_212_219_date_code.month_digits.data(), 2);
        std::memcpy(target + 216, programming.byte_212_219_date_code.day_digits.data(), 2);
        std::memcpy(target + 218, programming.byte_212_219_date_code.lot_code.data(), 2);

        target[220] =
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.temperature_monitoring_implemented_bit_5) << 5 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.supply_voltage_monitoring_implemented_bit_4) << 4 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.received_power_measurement_is_average_bit_3) << 3 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.transmitter_power_measurement_supported_bit_2) << 2 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.reserved_bit_1) << 1 |
            (unsigned char)(programming.byte_220_diagnostic_monitoring_type.reserved_bit_0) << 0;

        target[221] =
            (unsigned char)(programming.byte_221_enhanced_options.reserved_bit_7) << 7 |
            (unsigned char)(programming.byte_221_enhanced_options.reserved_bit_6) << 6 |
            (unsigned char)(programming.byte_221_enhanced_options.reserved_bit_5) << 5 |
            (unsigned char)(programming.byte_221_enhanced_options.initialization_complete_flag_implemented_bit_4) << 4 |
            (unsigned char)(programming.byte_221_enhanced_options.rate_selection_is_implemented_using_extended_rate_selection_bit_3) << 3 |
            (unsigned char)(programming.byte_221_enhanced_options.reserved_bit_2) << 2 |
            (unsigned char)(programming.byte_221_enhanced_options.readiness_flag_implemented_bit_1) << 1 |
            (unsigned char)(programming.byte_221_enhanced_options.software_reset_implemented_bit_0) << 0;

        target[222] = programming.byte_222_extended_baud_rate_in_250_mbaud;
        
        if(CC_EXTDirective == Standards::common::ChecksumDirective::AUTO_CALCULATE_FROM_CONTENT) {
            target[223] = calculateCC_EXTChecksum(target);
        } else if(CC_EXTDirective == Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING) {
            target[223] = programming.byte_223_CC_EXT;
        } else {
            throw std::runtime_error("Unhandled enum case!");
        }

        std::memcpy(target + 224, programming.byte_224_255_vendor_specific.data(), 32);
    }
}