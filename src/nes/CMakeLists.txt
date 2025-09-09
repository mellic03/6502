cmake_minimum_required(VERSION 3.5)

file(
    GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mapper/*.cpp"
)

add_library(
    memu_nes OBJECT
    ${SOURCES}
)
