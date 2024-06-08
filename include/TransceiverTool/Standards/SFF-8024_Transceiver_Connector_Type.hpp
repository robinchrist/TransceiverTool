#pragma once
#include <string>
#include <array>

namespace TransceiverTool::Standards::SFF8024 {

    enum class TransceiverConnectorType {
        Unknown_or_unspecified,
        SC_Subscriber_Connector,
        Fibre_Channel_Style_1_copper_connector,
        Fibre_Channel_Style_2_copper_connector,
        BNC_TNC_Bayonet_Threaded_Neill_Concelman,
        Fibre_Channel_coax_headers,
        Fiber_Jack,
        LC_Lucent_Connector,
        MT_RJ_Mechanical_Transfer_Registered_Jack,
        MU_Multiple_Optical,
        SG,
        Optical_Pigtail,
        MPO_1x12_Multifiber_Parallel_Optic,
        MPO_2x16,
        HSSDC_II_High_Speed_Serial_Data_Connector,
        Copper_pigtail,
        RJ45_Registered_Jack,
        No_separable_connector,
        MXC_2x16,
        CS_optical_connector,
        SN_previously_Mini_CS_optical_connector,
        MPO_2x12,
        MPO_1x16
    };

    struct TransceiverConnectorTypeAssignedValue {
        TransceiverConnectorType enum_value;
        std::string name;
        unsigned char value;
    };

    //SFF-8024 Rev 4.11 Table 4-3 Connector Types
    inline const std::array<TransceiverConnectorTypeAssignedValue, 23> TransceiverConnectorTypeAssignedValues {{
        {TransceiverConnectorType::Unknown_or_unspecified, "Unknown or unspecified", 0x00},
        {TransceiverConnectorType::SC_Subscriber_Connector, "SC (Subscriber Connector)", 0x01},
        {TransceiverConnectorType::Fibre_Channel_Style_1_copper_connector, "Fibre Channel Style 1 copper connector", 0x02},
        {TransceiverConnectorType::Fibre_Channel_Style_2_copper_connector, "Fibre Channel Style 2 copper connector", 0x03},
        {TransceiverConnectorType::BNC_TNC_Bayonet_Threaded_Neill_Concelman, "BNC/TNC (Bayonet/Threaded Neill-Concelman)", 0x04},
        {TransceiverConnectorType::Fibre_Channel_coax_headers, "Fibre Channel coax headers", 0x05},
        {TransceiverConnectorType::Fiber_Jack, "Fiber Jack", 0x06},
        {TransceiverConnectorType::LC_Lucent_Connector, "LC (Lucent Connector)", 0x07},
        {TransceiverConnectorType::MT_RJ_Mechanical_Transfer_Registered_Jack, "MT-RJ (Mechanical Transfer - Registered Jack)", 0x08},
        {TransceiverConnectorType::MU_Multiple_Optical, "MU (Multiple Optical)", 0x09},
        {TransceiverConnectorType::SG, "SG", 0x0A},
        {TransceiverConnectorType::Optical_Pigtail, "Optical Pigtail", 0x0B},
        {TransceiverConnectorType::MPO_1x12_Multifiber_Parallel_Optic, "MPO 1x12 (Multifiber Parallel Optic)", 0x0C},
        {TransceiverConnectorType::MPO_2x16, "MPO 2x16", 0x0D},
        //Gap_0x0Eh-0x1F: Reserved
        {TransceiverConnectorType::HSSDC_II_High_Speed_Serial_Data_Connector, "HSSDC II (High Speed Serial Data Connector)", 0x20},
        {TransceiverConnectorType::Copper_pigtail, "Copper pigtail", 0x21},
        {TransceiverConnectorType::RJ45_Registered_Jack, "RJ45 (Registered Jack)", 0x22},
        {TransceiverConnectorType::No_separable_connector, "No separable connector", 0x23},
        {TransceiverConnectorType::MXC_2x16, "MXC 2x16", 0x24},
        {TransceiverConnectorType::CS_optical_connector, "CS optical connector", 0x25},
        {TransceiverConnectorType::SN_previously_Mini_CS_optical_connector, "SN (previously Mini CS) optical connector", 0x26},
        {TransceiverConnectorType::MPO_2x12, "MPO 2x12", 0x27},
        {TransceiverConnectorType::MPO_1x16, "MPO 1x16", 0x28},
    }};

}