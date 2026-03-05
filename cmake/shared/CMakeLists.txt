cmake_minimum_required(VERSION 3.26)
project(byteweasel C)

set(CMAKE_C_STANDARD 11)

set(BYTEWEASEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../headers)
file(GLOB LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/byteweasel.c")

add_library(byteweasel SHARED ${LIB_SOURCES})
target_include_directories(byteweasel PUBLIC ${BYTEWEASEL_HEADERS})
target_compile_options(byteweasel PRIVATE -g)

set_target_properties(byteweasel PROPERTIES OUTPUT_NAME "byteweasel_shared")

if(WIN32)
    set_target_properties(byteweasel PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.import_libs"
    )
    add_custom_command(TARGET byteweasel POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/.import_libs/libbyteweasel_shared.dll.a"
            "${CMAKE_BINARY_DIR}/libbyteweasel_shared.dll.a"
    )
    target_compile_definitions(byteweasel PRIVATE BYTEWEASEL_EXPORTS)
else()
    set_target_properties(byteweasel PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()