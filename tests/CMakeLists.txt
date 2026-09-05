# SPDX-FileCopyrightText: 2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0

find_package(nlohmann_json_schema_validator REQUIRED)
add_library(schema_contract STATIC SchemaContract.cpp)
target_link_libraries(schema_contract PUBLIC TransceiverToolCore nlohmann_json_schema_validator)
target_include_directories(schema_contract PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(schema_contract PRIVATE TRANSCEIVERTOOL_SCHEMA_DIR="${PROJECT_SOURCE_DIR}/schemas")

foreach(standard IN ITEMS 8472 8636)
    add_executable(schema_roundtrip_${standard} RoundtripCases.cpp ../fuzz/Fuzz_SFF${standard}_Roundtrip.cpp)
    target_link_libraries(schema_roundtrip_${standard} PRIVATE schema_contract)
    add_test(NAME schema_roundtrip_${standard} COMMAND schema_roundtrip_${standard})
endforeach()

add_executable(schema_input_cases InputCases.cpp)
target_link_libraries(schema_input_cases PRIVATE schema_contract)
add_test(NAME schema_input_cases COMMAND schema_input_cases)

add_executable(schema_enums SchemaEnums.cpp)
target_link_libraries(schema_enums PRIVATE TransceiverToolCore)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME schema_enums_current
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/schema_enums.py" $<TARGET_FILE:schema_enums>)
