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

# Add the main server executable target
add_executable(TransceiverTool)

#target_sources called in CMakeLists.txt in src directory
add_subdirectory(src)

# Other non-cpu-optimisation related global compile flags
#cpu-optimisation flags will be in the next section
target_compile_options(TransceiverTool PUBLIC "$<$<CONFIG:Debug>:-g3;-O0>")
target_compile_options(TransceiverTool PUBLIC "$<$<CONFIG:RelWithDebInfo>:-g3;-O3>")
target_compile_options(TransceiverTool PUBLIC "$<$<CONFIG:Release>:-O3>")



# libstdc++ requires us to link against stdc++fs to use the filesystem
target_link_libraries(TransceiverTool PUBLIC stdc++fs)


target_link_libraries(TransceiverTool PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(TransceiverTool PUBLIC fmt::fmt)
target_link_libraries(TransceiverTool PUBLIC cppcodec::cppcodec)
target_link_libraries(TransceiverTool PUBLIC CLI11::CLI11)



target_include_directories(TransceiverTool PUBLIC include)
target_include_directories(TransceiverTool PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/generated")
