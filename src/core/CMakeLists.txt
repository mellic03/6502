cmake_minimum_required(VERSION 3.5)


file(
    GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(
    memu_core OBJECT
    ${SOURCES}
)

target_link_libraries(
    memu_core PRIVATE
    SDL3::SDL3
)
