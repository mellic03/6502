cmake_minimum_required(VERSION 3.5)

project(nesemu LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g" )
find_package(SDL2 REQUIRED)

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
