cmake_minimum_required(VERSION 3.26)
project(byteweasel C)

set(CMAKE_C_STANDARD 11)

set(BYTEWEASEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../headers)
file(GLOB EXE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.c")

add_executable(byteweasel ${EXE_SOURCES})
target_include_directories(byteweasel PRIVATE ${BYTEWEASEL_HEADERS})
target_compile_definitions(byteweasel PRIVATE BYTEWEASEL_EXPORTS)
target_compile_options(byteweasel PRIVATE -g)

if(WIN32)
    set_target_properties(byteweasel PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()