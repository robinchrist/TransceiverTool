# SPDX-FileCopyrightText: 2024-2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0

# Only reached when TRANSCEIVERTOOL_BUILD_FUZZERS is ON, which also asserts that the compiler is Clang
# and puts coverage instrumentation and the sanitizers on TransceiverToolCore.

# -fsanitize=fuzzer (as opposed to fuzzer-no-link on the core library) is what pulls in libFuzzer's main()
function(transceivertool_add_fuzzer target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE TransceiverToolCore)
    target_compile_options(${target} PRIVATE -fsanitize=fuzzer)
    target_link_options(${target} PRIVATE -fsanitize=fuzzer)
endfunction()

transceivertool_add_fuzzer(fuzz_sff8472_roundtrip Fuzz_SFF8472_Roundtrip.cpp)
transceivertool_add_fuzzer(fuzz_sff8636_roundtrip Fuzz_SFF8636_Roundtrip.cpp)
transceivertool_add_fuzzer(fuzz_json_input Fuzz_JSON_Input.cpp)
target_link_libraries(fuzz_sff8472_roundtrip PRIVATE schema_contract)
target_link_libraries(fuzz_sff8636_roundtrip PRIVATE schema_contract)
