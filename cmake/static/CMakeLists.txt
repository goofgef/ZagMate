cmake_minimum_required(VERSION 3.26)
project(byteweasel C)

set(CMAKE_C_STANDARD 11)

set(BYTEWEASEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../../headers)
file(GLOB LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../src/byteweasel.c")

add_library(byteweasel STATIC ${LIB_SOURCES})
target_compile_definitions(byteweasel PRIVATE BYTEWEASEL_EXPORTS)
target_include_directories(byteweasel PUBLIC ${BYTEWEASEL_HEADERS})
target_compile_options(byteweasel PRIVATE -g)

set_target_properties(byteweasel PROPERTIES OUTPUT_NAME "byteweasel_static")