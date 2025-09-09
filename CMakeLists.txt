cmake_minimum_required(VERSION 3.5)

project(nesemu LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g" )
find_package(SDL2 REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/core")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/nes")
# add_subdirectory("${CMAKE_SOURCE_DIR}/src/hw/mos6502")


file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/hw/mos6502/*.cpp"
)

add_executable(
    "system-mos-6502"
    "${CMAKE_SOURCE_DIR}/src/system_mos_6502.cpp"
    ${SOURCES}
)

target_link_libraries(
    "system-mos-6502" PRIVATE
    memu_core
)


add_executable(
    nesemu
    "${CMAKE_SOURCE_DIR}/src/system_nes.cpp"
    ${SOURCES}
)

target_link_libraries(
    nesemu PRIVATE
    memu_core
    memu_nes
)
