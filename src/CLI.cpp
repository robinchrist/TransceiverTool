#include "TransceiverTool/Standards/SFF-8472_LowerA0h.hpp"
#include "TransceiverTool/Version.hpp"
#include <cstddef>
#include <exception>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
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

#include "TransceiverTool/Standards/SFF-8472_Parser.hpp"
#include "TransceiverTool/Standards/SFF-8472_Pretty_Print.hpp"
#include "TransceiverTool/Standards/SFF-8472_Assembler.hpp"
#include "TransceiverTool/Standards/SFF-8472_Validation.hpp"
#include "TransceiverTool/Standards/SFF-8472_JSON.hpp"
#include <optional>

#include "CLI/CLI.hpp"

namespace TransceiverTool {

    

    enum class BinaryType {
        SFF_8472_128b,
        SFF_8636_128b,
        SFF_8636_256b
    };

    const std::map<std::string, BinaryType> BinaryTypeMap {
        {"8472-128b", BinaryType::SFF_8472_128b},
        {"8636-128b", BinaryType::SFF_8636_128b},
        {"8636-256b", BinaryType::SFF_8636_256b}
    };

    //Will return 128 bytes for SFF_8472_128b and 256 bytes for both SFF_8636_128b and SFF_8636_256b
    std::vector<std::byte> collectInput(BinaryType binaryType, const std::optional<std::string>& filePath) {
        size_t numBytesToRead;
        switch(binaryType) {
            case BinaryType::SFF_8472_128b: numBytesToRead = 128; break;
            case BinaryType::SFF_8636_128b: numBytesToRead = 128; break;
            case BinaryType::SFF_8636_256b: numBytesToRead = 256; break;
            default: throw std::runtime_error("Non exhaustive enum switch");
        }

        std::vector<std::byte> buffer; buffer.resize(numBytesToRead);

        if(filePath.has_value()) {

            std::ifstream inStream;
            inStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

            try {
                inStream.open(filePath.value(), std::ios::in | std::ios::binary);
            } catch(const std::exception& e) {
                throw std::runtime_error(fmt::format("Failed to open path {}, exception {}", filePath.value(), e.what()));
            }

            try {
                inStream.read(reinterpret_cast<char*>(buffer.data()), numBytesToRead);
            } catch(const std::exception& e) {
                if(inStream.gcount() < numBytesToRead) {
                    throw std::runtime_error(fmt::format("Failed to read enough bytes from path {}, exception {}", filePath.value(), e.what()));
                }
            }
        } else {
            try {
                std::freopen(nullptr, "rb", stdin);

                if(std::ferror(stdin)) {
                    throw std::runtime_error(std::strerror(errno));
                }

                size_t readBytes = 0, totalReadBytes = 0;
                static_assert(sizeof(std::byte) == 1);

                while((readBytes = std::fread(buffer.data() + totalReadBytes, 1, buffer.size() - totalReadBytes, stdin)) > 0) {

                    if(std::ferror(stdin) && !std::feof(stdin)) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    totalReadBytes += readBytes;
                }

                if(totalReadBytes < buffer.size()) {
                    throw std::runtime_error(fmt::format("Not enough input, require exactly {} bytes", buffer.size()));
                }
            } catch(const std::exception& e) {
                throw;
            }
        }


        if(binaryType == BinaryType::SFF_8636_128b) {
            std::vector<std::byte> newBuffer(256, std::byte{0});
            std::memcpy(newBuffer.data() + 128, buffer.data(), 128);

            buffer = std::move(newBuffer);
        }

        return buffer;
    }

    std::vector<std::byte> collectInput(const std::optional<std::string>& filePath) {

        std::vector<std::byte> buffer;

        if(filePath.has_value()) {

            std::ifstream inStream;
            inStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
            try {
                inStream.open(filePath.value(), std::ios::in | std::ios::binary);
            } catch(const std::exception& e) {
                throw std::runtime_error(fmt::format("Failed to open path {}, exception {}", filePath.value(), e.what()));
            }
        
            inStream.ignore( std::numeric_limits<std::streamsize>::max() );
            std::streamsize length = inStream.gcount();
            inStream.clear();   //  Since ignore will have set eof.
            inStream.seekg( 0, std::ios_base::beg );

            buffer.resize(length);
            
            try {
                inStream.read(reinterpret_cast<char*>(buffer.data()), length);
            } catch(const std::exception& e) {
                if(inStream.gcount() < length) {
                    throw std::runtime_error(fmt::format("Failed to read enough bytes from path {}, exception {}", filePath.value(), e.what()));
                }
            }
        } else {
            try {
                std::freopen(nullptr, "rb", stdin);

                if(std::ferror(stdin)) {
                    throw std::runtime_error(std::strerror(errno));
                }

                size_t readBytes = 0;
                std::vector<std::byte> tempBuffer; tempBuffer.resize(32*1024); //32kByte should be enough
                static_assert(sizeof(std::byte) == 1);

                while((readBytes = std::fread(tempBuffer.data(), 1, tempBuffer.size(), stdin)) > 0) {

                    if(std::ferror(stdin) && !std::feof(stdin)) {
                        throw std::runtime_error(std::strerror(errno));
                    }
                    buffer.insert(buffer.end(), tempBuffer.data(), tempBuffer.data() + readBytes);
                }
            } catch(const std::exception& e) {
                throw;
            }
        }

        return buffer;
    }

    class DecodeCommand {
        public:
            DecodeCommand(CLI::App& app) {

                initCommand(app);
            }

            CLI::App* subcommand;

        private:
            void initCommand(CLI::App& app) {
                CLI::App* decodeSubcommand = app.add_subcommand("decode", "Decode / parse binary transceiver programming to JSON and/or pretty print");
                subcommand = decodeSubcommand;

                decodeSubcommand->add_option("--binary-type", binaryType, 
                "Type of binary input\n"
                        "8472-128b: SFF-8472 (SFP/+/28) and 128 byte, \n"
                        "8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or\n"
                        "8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed"
                )
                    ->required()
                    ->transform(CLI::CheckedTransformer(BinaryTypeMap));

                decodeSubcommand->add_flag("--fiber-mode,!--copper-mode", fiberMode, "Specify meaning of bytes that can have multiple meanings, default fiber-mode");
                decodeSubcommand->add_option("--from-file", fromFile, "File path from which the binary shall be read instead of stdin");
                decodeSubcommand->add_flag("--pretty-print", prettyPrint, "Pretty-prints the decoded / parsed binary programming to stdout");
                decodeSubcommand->add_flag("--json-to-stdout", JSONToStdout, "Print the decoded / parsed binary programming as JSON to stdout");
                decodeSubcommand->add_option("--pretty-print-to-file", prettyPrintToFile, "Write the pretty-print of the decoded / parsed binary programming to a file");
                decodeSubcommand->add_option("--json-to-file", JSONToFile, "Write the JSON of the decoded / parsed binary programming");

                decodeSubcommand->callback(std::bind(&DecodeCommand::runCommand, this));
            }

            void runCommand() {

                if(!prettyPrint && !JSONToStdout && !prettyPrintToFile.has_value() && !JSONToFile.has_value()) {
                    std::cout << "Warning: No output options specified, exiting..." << std::endl;
                    return;
                }

                std::vector<std::byte> buffer = collectInput(binaryType, fromFile);
                std::string prettyPrintStr, serialisedJSON;


                if(binaryType == BinaryType::SFF_8472_128b) {
                    auto parsedStruct = TransceiverTool::Standards::SFF8472::parseBytesToStruct(reinterpret_cast<unsigned char const *>(buffer.data()));

                    if(prettyPrint || prettyPrintToFile.has_value()) {
                        prettyPrintStr = TransceiverTool::Standards::SFF8472::prettyPrintProgramming(parsedStruct, fiberMode);
                    }

                    if(JSONToStdout || JSONToFile.has_value()) {
                        nlohmann::ordered_json j;
                        TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hToJSON(j, parsedStruct, fiberMode);

                        serialisedJSON = j.dump(4);
                    }
                } else {
                    //SFF_8636_128b or SFF_8636_256b

                    auto parsedStruct = TransceiverTool::Standards::SFF8636::parseBytesToStruct(reinterpret_cast<unsigned char const *>(buffer.data()));

                    if(prettyPrint || prettyPrintToFile.has_value()) {
                        prettyPrintStr = TransceiverTool::Standards::SFF8636::prettyPrintProgramming(parsedStruct, fiberMode);
                    }

                    if(JSONToStdout || JSONToFile.has_value()) {
                        nlohmann::ordered_json j;
                        TransceiverTool::Standards::SFF8636::SFF8636_Upper00hToJSON(j, parsedStruct, fiberMode);

                        serialisedJSON = j.dump(4);
                    }
                }

                if(prettyPrint) {
                    std::cout << prettyPrintStr << std::endl;
                }

                if(prettyPrintToFile.has_value()) {
                    try {
                        std::ofstream outStream;
                        outStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
                        outStream.open(prettyPrintToFile.value(), std::ios::out);
                        outStream.write(prettyPrintStr.data(), prettyPrintStr.size());
                        outStream.flush();
                        outStream.close();
                    } catch(const std::exception& e) {
                        std::cerr << "Failed to write Pretty Print to path " << prettyPrintToFile.value() << " because " << e.what() << "\n";
                    }
                }

                if(JSONToStdout) {
                    std::cout << serialisedJSON << std::endl;
                }

                if(JSONToFile.has_value()) {
                    try {
                        std::ofstream outStream;
                        outStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
                        outStream.open(JSONToFile.value(), std::ios::out);
                        outStream.write(serialisedJSON.data(), serialisedJSON.size());
                        outStream.flush();
                        outStream.close();
                    } catch(const std::exception& e) {
                        std::cerr << "Failed to write JSON to path " << prettyPrintToFile.value() << " because " << e.what() << "\n";
                    }
                }
            }

            bool fiberMode = true;
            BinaryType binaryType;
            std::optional<std::string> fromFile = std::nullopt;
            bool prettyPrint = false;
            bool JSONToStdout = false;
            std::optional<std::string> prettyPrintToFile = std::nullopt;
            std::optional<std::string> JSONToFile = std::nullopt;
    };


    class EncodeCommand {
        public:
            EncodeCommand(CLI::App& app) {

                initCommand(app);
            }

            CLI::App* subcommand;

        private:
            void initCommand(CLI::App& app) {
                CLI::App* encodeSubcommand = app.add_subcommand("encode", "Encode / assembly binary transceiver programming from JSON");
                subcommand = encodeSubcommand;

                encodeSubcommand->add_option("--binary-type", binaryType, 
                "Type of binary output\n"
                        "8472-128b: SFF-8472 (SFP/+/28) and 128 byte, \n"
                        "8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or\n"
                        "8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed"
                )
                    ->required()
                    ->transform(CLI::CheckedTransformer(BinaryTypeMap));

                encodeSubcommand->add_option("--from-file", fromFile, "File path from which the JSON shall be read instead of stdin");
                encodeSubcommand->add_option("--to-file", toFile, "File path to which the binary shall be written instead of stdout");
                encodeSubcommand->add_flag("--validate", validate, "Try to validate programming, don't write file if any errors occur");

                encodeSubcommand->callback(std::bind(&EncodeCommand::runCommand, this));
            }

            void runCommand() {

                std::vector<std::byte> JSONInput = collectInput(fromFile);

                nlohmann::json parsedJSON;
                std::string type;
                try {
                    parsedJSON = nlohmann::json::parse(JSONInput);
                    type = parsedJSON.at("Type").template get<std::string>();
                } catch(const std::exception& e) {
                    throw std::runtime_error(fmt::format("Failed to parse JSON: {}", e.what()));
                }

                std::vector<std::byte> reassembled;

                if(binaryType == BinaryType::SFF_8472_128b) {
                    if(type != Standards::SFF8472::SFF8472_JSON_TYPE) {
                        throw std::runtime_error(fmt::format("JSON has wrong type, expected {} but is {}", Standards::SFF8472::SFF8472_JSON_TYPE, type));
                    }

                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h programming;
                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hFromJSON(parsedJSON, programming);

                    if(validate) {
                        auto validationResult = TransceiverTool::Standards::SFF8472::Validation::validateSFF8472_LowerA0h(programming);

                        if(!validationResult.warnings.empty()) {
                            std::cout << "Warnings:" << std::endl;
                            for(const auto& warning : validationResult.warnings) {
                                std::cout << warning << std::endl;
                            }
                        }

                        if(!validationResult.errors.empty()) {
                            std::cerr << "Errors:" << std::endl;
                            for(const auto& error : validationResult.errors) {
                                std::cerr << error << std::endl;
                            }
                            throw std::runtime_error("Validation failed with errors, not writing binary");
                        }
                    }

                    reassembled.resize(128, std::byte{0});
                    TransceiverTool::Standards::SFF8472::assembleToBinary(
                        reinterpret_cast<unsigned char *>(reassembled.data()),
                        programming,
                        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
                        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
                    );

                } else {
                    //SFF8636 128b or 256b
                    if(type != Standards::SFF8636::SFF8636_JSON_TYPE) {
                        throw std::runtime_error(fmt::format("JSON has wrong type, expected {} but is {}", Standards::SFF8636::SFF8636_JSON_TYPE, type));
                    }

                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h programming;
                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(parsedJSON, programming);

                    if(validate) {
                        auto validationResult = TransceiverTool::Standards::SFF8636::Validation::validateSFF8636_Upper00h(programming);

                        if(!validationResult.warnings.empty()) {
                            std::cout << "Warnings:" << std::endl;
                            for(const auto& warning : validationResult.warnings) {
                                std::cout << warning << std::endl;
                            }
                        }

                        if(!validationResult.errors.empty()) {
                            std::cerr << "Errors:" << std::endl;
                            for(const auto& error : validationResult.errors) {
                                std::cerr << error << std::endl;
                            }
                            throw std::runtime_error("Validation failed with errors, not writing binary");
                        }
                    }

                    reassembled.resize(256, std::byte{0});
                    TransceiverTool::Standards::SFF8636::assembleToBinary(
                        reinterpret_cast<unsigned char *>(reassembled.data()),
                        programming,
                        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
                        TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
                    );

                    if(binaryType == BinaryType::SFF_8636_128b) {
                        std::vector<std::byte> temp; temp.resize(128);
                        std::memcpy(temp.data(), reassembled.data() + 128, 128);
                        reassembled = std::move(temp);
                    }
                }

                if(toFile.has_value()) {
                    try {
                        std::ofstream outStream;
                        outStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
                        outStream.open(toFile.value(), std::ios::out | std::ios::binary);
                        outStream.write(reinterpret_cast<char*>(reassembled.data()), reassembled.size());
                        outStream.flush();
                        outStream.close();
                    } catch(const std::exception& e) {
                        std::cerr << "Failed to write reassembled binary to path " << toFile.value() << " because " << e.what() << "\n";
                    }
                }

            }

            BinaryType binaryType;
            std::optional<std::string> fromFile = std::nullopt;
            std::optional<std::string> toFile = std::nullopt;
            bool validate = false;
    };

    class PrettyPrintJSONCommand {
        public:
            PrettyPrintJSONCommand(CLI::App& app) {

                initCommand(app);
            }

            CLI::App* subcommand;

        private:
            void initCommand(CLI::App& app) {
                CLI::App* prettyPrintJSONSubcommand = app.add_subcommand("pretty-print-json", "Pretty print a JSON file of a decoded programming");
                subcommand = prettyPrintJSONSubcommand;

                prettyPrintJSONSubcommand->add_flag("--fiber-mode,!--copper-mode", fiberMode, "Specify meaning of bytes that can have multiple meanings, default fiber-mode");
                prettyPrintJSONSubcommand->add_option("--from-file", fromFile, "File path from which the JSON shall be read instead of stdin");
                prettyPrintJSONSubcommand->add_option("--to-file", toFile, "File path to which the pretty print shall be written instead of stdout");

                prettyPrintJSONSubcommand->callback(std::bind(&PrettyPrintJSONCommand::runCommand, this));
            }

            void runCommand() {

                std::vector<std::byte> JSONInput = collectInput(fromFile);

                nlohmann::json parsedJSON;
                std::string type;
                try {
                    parsedJSON = nlohmann::json::parse(JSONInput);
                    type = parsedJSON.at("Type").template get<std::string>();
                } catch(const std::exception& e) {
                    throw std::runtime_error(fmt::format("Failed to parse JSON: {}", e.what()));
                }

                std::string prettyPrintStr;

                if(type == Standards::SFF8472::SFF8472_JSON_TYPE) {
                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h programming;
                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hFromJSON(parsedJSON, programming);

                    prettyPrintStr = TransceiverTool::Standards::SFF8472::prettyPrintProgramming(programming, fiberMode);
                } else if(type == Standards::SFF8636::SFF8636_JSON_TYPE) {
                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h programming;
                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(parsedJSON, programming);

                    prettyPrintStr = TransceiverTool::Standards::SFF8636::prettyPrintProgramming(programming, fiberMode);
                } else {
                    throw std::runtime_error(fmt::format("JSON has unknown Type {}", type));
                }

                if(toFile.has_value()) {
                    try {
                        std::ofstream outStream;
                        outStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
                        outStream.open(toFile.value(), std::ios::out);
                        outStream.write(prettyPrintStr.data(), prettyPrintStr.size());
                        outStream.flush();
                        outStream.close();
                    } catch(const std::exception& e) {
                        std::cerr << "Failed to write Pretty Print to path " << toFile.value() << " because " << e.what() << "\n";
                    }
                } else {
                    std::cout << prettyPrintStr << std::endl;
                }
            }

            bool fiberMode = true;
            std::optional<std::string> fromFile = std::nullopt;
            std::optional<std::string> toFile = std::nullopt;
    };

    class ValidateJSONCommand {
        public:
            ValidateJSONCommand(CLI::App& app) {

                initCommand(app);
            }

            CLI::App* subcommand;

        private:
            void initCommand(CLI::App& app) {
                CLI::App* validateJSONSubcommand = app.add_subcommand("validate-json", "Validate a JSON file of a decoded programming");
                subcommand = validateJSONSubcommand;

                validateJSONSubcommand->add_option("--from-file", fromFile, "File path from which the JSON shall be read instead of stdin");

                validateJSONSubcommand->callback(std::bind(&ValidateJSONCommand::runCommand, this));
            }

            void runCommand() {

                std::vector<std::byte> JSONInput = collectInput(fromFile);

                nlohmann::json parsedJSON;
                std::string type;
                try {
                    parsedJSON = nlohmann::json::parse(JSONInput);
                    type = parsedJSON.at("Type").template get<std::string>();
                } catch(const std::exception& e) {
                    throw std::runtime_error(fmt::format("Failed to parse JSON: {}", e.what()));
                }

                std::string prettyPrintStr;

                if(type == Standards::SFF8472::SFF8472_JSON_TYPE) {
                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h programming;
                    TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hFromJSON(parsedJSON, programming);

                    auto validationResult = TransceiverTool::Standards::SFF8472::Validation::validateSFF8472_LowerA0h(programming);

                    if(!validationResult.warnings.empty()) {
                        std::cout << "Warnings:" << std::endl;
                        for(const auto& warning : validationResult.warnings) {
                            std::cout << warning << std::endl;
                        }
                    }

                    if(!validationResult.errors.empty()) {
                        std::cerr << "Errors:" << std::endl;
                        for(const auto& error : validationResult.errors) {
                            std::cerr << error << std::endl;
                        }
                    }
                } else if(type == Standards::SFF8636::SFF8636_JSON_TYPE) {
                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00h programming;
                    TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(parsedJSON, programming);

                    auto validationResult = TransceiverTool::Standards::SFF8636::Validation::validateSFF8636_Upper00h(programming);

                    if(!validationResult.warnings.empty()) {
                        std::cout << "Warnings:" << std::endl;
                        for(const auto& warning : validationResult.warnings) {
                            std::cout << warning << std::endl;
                        }
                    }

                    if(!validationResult.errors.empty()) {
                        std::cerr << "Errors:" << std::endl;
                        for(const auto& error : validationResult.errors) {
                            std::cerr << error << std::endl;
                        }
                    }
                } else {
                    throw std::runtime_error(fmt::format("JSON has unknown Type {}", type));
                }

            }

            std::optional<std::string> fromFile = std::nullopt;
    };

    class RoundtripTestCommand {
        public:
            RoundtripTestCommand(CLI::App& app) {

                initCommand(app);
            }

            CLI::App* subcommand;

        private:
            void initCommand(CLI::App& app) {
                CLI::App* roundtripSubcommand = app.add_subcommand("roundtrip-test", "Perform a roundtrip test (Binary -> JSON -> Binary)");
                subcommand = roundtripSubcommand;

                roundtripSubcommand->add_option("--binary-type", binaryType, 
                "Type of binary input\n"
                        "8472-128b: SFF-8472 (SFP/+/28) and 128 byte, \n"
                        "8636-128b: SFF-8636 (QSFP/+/28) and 128 byte, aka upper page 00h or the last 128 bytes of page 00h or\n"
                        "8636-256b: SFF-8636 (QSFP/+/28) and 256 byte, aka full page 00h and first 128 bytes ignored / zeroed"
                )
                    ->required()
                    ->transform(CLI::CheckedTransformer(BinaryTypeMap));

                roundtripSubcommand->add_option("--from-file", fromFile, "File path from which the binary shall be read instead of stdin");

                roundtripSubcommand->callback(std::bind(&RoundtripTestCommand::runCommand, this));
            }

            void runCommand() {

                std::vector<std::byte> buffer = collectInput(binaryType, fromFile);

                try {
                    if(binaryType == BinaryType::SFF_8472_128b) {

                        auto test = [&buffer = std::as_const(buffer)](bool fiberMode) {
                            auto parsedStruct = TransceiverTool::Standards::SFF8472::parseBytesToStruct(reinterpret_cast<unsigned char const *>(buffer.data()));

                            nlohmann::ordered_json j;
                            TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hToJSON(j, parsedStruct, false);

                            std::string serialised = j.dump();

                            TransceiverTool::Standards::SFF8472::SFF8472_LowerA0h programmingRoundtrip;
                            try {
                                TransceiverTool::Standards::SFF8472::SFF8472_LowerA0hFromJSON(nlohmann::json::parse(serialised), programmingRoundtrip);
                            } catch(const std::exception& e) {
                                throw std::runtime_error(fmt::format("SFF8472_LowerA0hFromJSON for fiberMode = {} failed with exception {}", fiberMode, e.what()));
                            }

                            std::vector<unsigned char> reassembled; reassembled.resize(128, 0x00);
                            TransceiverTool::Standards::SFF8472::assembleToBinary(
                                reassembled.data(),
                                parsedStruct,
                                TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
                                TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
                            );

                            bool reassembleError = false;
                            for(int index = 0; index < 128; ++index) {
                                if(std::byte{reassembled[index]} != buffer[index]) {
                                    reassembleError = true;

                                    std::cerr << fmt::format("fiberMode = {}, bytes at index {} different. Should be {:#04x} but is {:#04x}\n", fiberMode, index, buffer[index], reassembled[index]);
                                }
                            }

                            if(reassembleError) throw std::runtime_error("comparison between original bytes and reassembled bytes failed");
                        };

                        try {
                            test(true);
                            test(false);
                        } catch(const std::exception& e) {
                            throw std::runtime_error(fmt::format("SFF-8472: {}", e.what()));
                        }


                    } else {
                        //SFF_8636_128b or SFF_8636_256b

                        auto test = [&buffer = std::as_const(buffer)](bool fiberMode) {
                            auto parsedStruct = TransceiverTool::Standards::SFF8636::parseBytesToStruct(reinterpret_cast<unsigned char const *>(buffer.data()));

                            nlohmann::ordered_json j;
                            TransceiverTool::Standards::SFF8636::SFF8636_Upper00hToJSON(j, parsedStruct, fiberMode);

                            std::string serialised = j.dump();

                            TransceiverTool::Standards::SFF8636::SFF8636_Upper00h programmingRoundtrip;
                            try {
                                TransceiverTool::Standards::SFF8636::SFF8636_Upper00hFromJSON(nlohmann::json::parse(serialised), programmingRoundtrip);
                            } catch(const std::exception& e) {
                                throw std::runtime_error(fmt::format("SFF8636_LowerA0hFromJSON for fiberMode = {} failed with exception {}", fiberMode, e.what()));
                            }

                            std::vector<unsigned char> reassembled; reassembled.resize(256, 0x00);
                            TransceiverTool::Standards::SFF8636::assembleToBinary(
                                reassembled.data(),
                                parsedStruct,
                                TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING,
                                TransceiverTool::Standards::common::ChecksumDirective::MANUAL_USE_VALUE_IN_PROGRAMMING
                            );

                            bool reassembleError = false;
                            for(int index = 128; index < 256; ++index) {
                                if(std::byte{reassembled[index]} != buffer[index]) {
                                    reassembleError = true;

                                    std::cerr << fmt::format("fiberMode = {}, bytes at index {} different. Should be {:#04x} but is {:#04x}\n", fiberMode, index, buffer[index], reassembled[index]);
                                }
                            }

                            if(reassembleError) throw std::runtime_error("comparison between original bytes and reassembled bytes failed");
                        };

                        try {
                            test(true);
                            test(false);
                        } catch(const std::exception& e) {
                            throw std::runtime_error(fmt::format("SFF-8636: {}", e.what()));
                        }
                    }
                } catch(const std::exception& e) {
                    throw std::runtime_error(fmt::format("Roundtrip test failed, exception {}", e.what()));
                }

                std::cout << "Roundtrip test succeeded!" << std::endl;
                
            }

            BinaryType binaryType;
            std::optional<std::string> fromFile = std::nullopt;
    };
}

int main(int argc, char **argv) {
    
    CLI::App app{
        fmt::format("TransceiverTool v{} - Encode / Assemble from JSON, Decode / Parse to JSON and Pretty Print SFF-8472 SFP/+/28 and SFF-8636 QSFP/+/28 programmings / EEPROMs!",
            TRANSCEIVERTOOL_VERSION
        )
    };

    app.set_version_flag("--version", fmt::format("TransceiverTool v{}", TRANSCEIVERTOOL_VERSION));

    //Print help-all by default
    app.set_help_flag("");
    app.set_help_all_flag("--help", "Print this help message and exit");

    TransceiverTool::DecodeCommand decodeCommand { app };
    TransceiverTool::EncodeCommand encodeCommand { app };
    TransceiverTool::PrettyPrintJSONCommand prettyPrintJSONCommand { app };
    TransceiverTool::ValidateJSONCommand validateJSONCommand { app };
    TransceiverTool::RoundtripTestCommand roundtripCommand { app };


    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        return app.exit(e);
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return -1;
    }

    if(!decodeCommand.subcommand->parsed() && !encodeCommand.subcommand->parsed() && !prettyPrintJSONCommand.subcommand->parsed() && !validateJSONCommand.subcommand->parsed() && !roundtripCommand.subcommand->parsed()) {
        std::cout << app.help("", CLI::AppFormatMode::All) << std::endl;
    }

    return 0;
}