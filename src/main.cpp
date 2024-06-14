#include <cstddef>
#include <exception>
#include <nlohmann/json_fwd.hpp>
#include <variant>
#include <string>
#include <array>
#include <vector>
#include <fmt/core.h>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>


#include "TransceiverTool/Standards/SFF-8636_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8636_Upper00h.hpp"
#include "TransceiverTool/Standards/SFF-8636_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8636_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8636_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8636_JSON.hpp"


int main() {
    
    std::ifstream file("/data/dev/TransceiverTool_Go/TransceiverDatabase/FS/QSFP28-LR-100G/QSFP28-LR-100G_Mellanox.bin_2", std::ios::in | std::ios::binary);
    file.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize length = file.gcount();
    file.clear();   //  Since ignore will have set eof.
    file.seekg( 0, std::ios_base::beg );

    if(length < 256) {
        std::cerr << "Length smaller than 256, exiting... \n";
        return -1;
    }

    std::vector<unsigned char> buffer; buffer.resize(length);

    file.read(reinterpret_cast<char*>(buffer.data()), length);

    file.close();


    std::cout << "Decoding..." << std::endl;
    auto parsedStruct = TransceiverTool::Standards::SFF8636::parseBytesToStruct(buffer.data());

    std::cout << "Sucessfully decoded!" << std::endl;

    std::string prettyPrint = TransceiverTool::Standards::SFF8636::prettyPrintProgramming(parsedStruct, true, true);

    std::cout << "Decoded:" << std::endl;
    std::cout << prettyPrint << std::endl;
    std::cout << std::endl;

    std::cout << "Verifying..." << std::endl;

    auto validationResult = TransceiverTool::Standards::SFF8636::Validation::validateSFF8636_Upper00h(parsedStruct);
    std::cout << "Errors? " << !validationResult.errors.empty() << std::endl;
    std::cout << "Warnings? " << !validationResult.warnings.empty() << std::endl;

    std::cout << "Errors:" << std::endl;
    for(const auto& error : validationResult.errors) {
        std::cout << error << std::endl;
    }

    std::cout << "Warnings:" << std::endl;
    for(const auto& warning : validationResult.warnings) {
        std::cout << warning << std::endl;
    }

    std::cout << "Checking reassembly" << std::endl;
    std::vector<unsigned char> reassembled; reassembled.resize(256, 0x00);
    TransceiverTool::Standards::SFF8636::assembleToBinary(
        reassembled.data(),
        parsedStruct,
        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
    );

    bool reassembleError = false;
    for(int index = 128; index < 256; ++index) {
        if(reassembled[index] != buffer[index]) {
            reassembleError = true;

            std::cout << fmt::format("Error: Bytes at index {} different. Should be {:#04x} but is {:#04x}", index, buffer[index], reassembled[index]);
        }
    }

    if(reassembleError) return -1;

    std::cout << "Reassembly succeeded!" << std::endl;


    nlohmann::ordered_json j;
    TransceiverTool::Standards::SFF8636::SFF8636_Upper00hToJSON(j, parsedStruct, false);

    std::cout << std::setw(4) << j << std::endl;

    std::string serialised = j.dump();


    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h programmingRoundtrip;
    TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(nlohmann::json::parse(serialised), programmingRoundtrip);

    //prettyPrint = TransceiverTool::Standards::SFF8636::prettyPrintProgramming(programmingRoundtrip, true, true);

    //std::cout << "Decoded:" << std::endl;
    //std::cout << prettyPrint << std::endl;

    std::cout << "Checking reassembly from JSON" << std::endl;
    reassembled.clear();
    reassembled.resize(256, 0x00);

    TransceiverTool::Standards::SFF8636::assembleToBinary(
        reassembled.data(),
        programmingRoundtrip,
        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
    );

    reassembleError = false;
    for(int index = 128; index < 256; ++index) {
        if(reassembled[index] != buffer[index]) {
            reassembleError = true;

            std::cout << fmt::format("Error: Bytes at index {} different. Should be {:#04x} but is {:#04x}\n", index, buffer[index], reassembled[index]);
        }
    }

    if(reassembleError) return -1;

    std::cout << "Reassembly succeeded!" << std::endl;

    return 0;
}