# Make sure we use a relatively recent version - 3.27 is Nov 2023
cmake_minimum_required(VERSION 3.27)

project ("Transceiver Tool" VERSION 0.0.1)

# Make sure we group dependencies here
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(cppcodec REQUIRED)
find_package(CLI11 REQUIRED)


#set preferences
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Version.hpp is generated so that project(... VERSION ...) above stays the single source of truth
configure_file(
    include/TransceiverTool/Version.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/generated/TransceiverTool/Version.hpp"
    @ONLY
)

option(TRANSCEIVERTOOL_BUILD_FUZZERS "Build the libFuzzer fuzz targets (requires Clang)" OFF)

# All standards code lives in a library so that the CLI and the fuzz targets can share it.
# CLI.cpp is the only source of the executable itself.
add_library(TransceiverToolCore STATIC)

# Add the main server executable target
add_executable(TransceiverTool)

#target_sources called in CMakeLists.txt in src directory
add_subdirectory(src)

# Other non-cpu-optimisation related global compile flags
#cpu-optimisation flags will be in the next section
target_compile_options(TransceiverToolCore PUBLIC "$<$<CONFIG:Debug>:-g3;-O0>")
target_compile_options(TransceiverToolCore PUBLIC "$<$<CONFIG:RelWithDebInfo>:-g3;-O3>")
target_compile_options(TransceiverToolCore PUBLIC "$<$<CONFIG:Release>:-O3>")



# libstdc++ requires us to link against stdc++fs to use the filesystem
target_link_libraries(TransceiverToolCore PUBLIC stdc++fs)


target_link_libraries(TransceiverToolCore PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(TransceiverToolCore PUBLIC fmt::fmt)
target_link_libraries(TransceiverToolCore PUBLIC cppcodec::cppcodec)



target_include_directories(TransceiverToolCore PUBLIC include)
target_include_directories(TransceiverToolCore PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/generated")


# Everything above is PUBLIC on the core library, so the executable inherits it
target_link_libraries(TransceiverTool PRIVATE TransceiverToolCore)
# CLI11 is only used by CLI.cpp, so it stays off the core library
target_link_libraries(TransceiverTool PRIVATE CLI11::CLI11)


if(TRANSCEIVERTOOL_BUILD_FUZZERS)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR
            "TRANSCEIVERTOOL_BUILD_FUZZERS requires Clang for libFuzzer, but the compiler is "
            "${CMAKE_CXX_COMPILER_ID}. Configure with the TransceiverTool-fuzzing conan profile."
        )
    endif()

    # The code under test needs coverage instrumentation for libFuzzer to make progress, and the
    # sanitizers are what turn a silently wrong parse into a reported crash. These are PUBLIC, so
    # a fuzzing build also produces a sanitized CLI binary - that build is for fuzzing, not for shipping.
    target_compile_options(TransceiverToolCore PUBLIC -fsanitize=fuzzer-no-link,address,undefined -fno-omit-frame-pointer)
    target_link_options(TransceiverToolCore PUBLIC -fsanitize=fuzzer-no-link,address,undefined)

    add_subdirectory(fuzz)
endif()
