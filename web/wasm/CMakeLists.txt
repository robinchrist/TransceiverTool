# SPDX-FileCopyrightText: 2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "Use the TransceiverTool-wasm Conan profile (see web/README.md)")
endif()

add_executable(transceiver_web Bridge.cpp)
target_link_libraries(transceiver_web PRIVATE TransceiverToolCore)
target_compile_options(TransceiverToolCore PUBLIC -fexceptions)
target_link_options(transceiver_web PRIVATE
    --bind -fexceptions -sDISABLE_EXCEPTION_CATCHING=0
    -sMODULARIZE=1 -sEXPORT_ES6=1 -sSINGLE_FILE=1 -sALLOW_MEMORY_GROWTH=1
    -sMAXIMUM_MEMORY=134217728 -sENVIRONMENT=web,node -sFILESYSTEM=0
)
set_target_properties(transceiver_web PROPERTIES OUTPUT_NAME "transceiver" SUFFIX ".mjs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../src/wasm")
