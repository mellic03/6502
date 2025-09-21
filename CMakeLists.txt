cmake_minimum_required(VERSION 3.5)

project(nesemu LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
set(CMAKE_CXX_STANDARD "20")
set(MEMU_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/NesEmu")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MEMU_OUTPUT_DIR})

find_package(SDL3 REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/core")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/hw")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/nes")
# add_subdirectory("${CMAKE_SOURCE_DIR}/src/hw/mos6502")

add_executable(
    nesemu
    "${CMAKE_SOURCE_DIR}/src/system_nes.cpp"
    ${SOURCES}
)

target_link_libraries(
    nesemu PRIVATE
    memu_core
    memu_hw
    memu_nes
)



add_executable(
    test_m6502
    "${CMAKE_SOURCE_DIR}/src/test_m6502.cpp"
    ${SOURCES}
)

target_link_libraries(
    test_m6502 PRIVATE
    memu_core
    memu_hw
    memu_nes
)



file(
    COPY
        "${CMAKE_SOURCE_DIR}/data/"
    DESTINATION
        "${MEMU_OUTPUT_DIR}/"
)
