cmake_minimum_required(VERSION 3.5)

file(
    GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/mostech/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mostech/6502/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ricoh/*.cpp"
)

add_library(
    memu_hw OBJECT
    ${SOURCES}
)
