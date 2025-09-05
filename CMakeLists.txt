cmake_minimum_required(VERSION 3.5)
project(nesemu LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g" )

find_package(SDL2 REQUIRED)

file(
    GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/6502/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/nes/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/nes/file/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/nes/mapper/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/hw/*.cpp"
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
)
