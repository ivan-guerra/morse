cmake_minimum_required(VERSION 3.13...3.25)

project(morse DESCRIPTION "a bidirectional morse code translator")

# Append the SDL2 Find* script paths to CMAKE_MODULE_PATH.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(MORSE_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING  "${PROJECT_NAME} binary directory.")
set(MORSE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include"
    CACHE STRING      "${PROJECT_NAME} include directory.")

set(COMMON_FLAGS
    -std=c++2a
)

set(WARNING_FLAGS
    -Wall
    -Wextra
    -Werror
    -pedantic
)

set(DEBUG_FLAGS
    ${COMMON_FLAGS}
    ${WARNING_FLAGS}
    -O0
    -g3
    -ggdb
    -fno-omit-frame-pointer
    -fsanitize=address
)

set(RELEASE_FLAGS
    ${COMMON_FLAGS}
    -O2
)

add_compile_options(
    "$<$<CONFIG:Release>:${RELEASE_FLAGS}>"
    "$<$<CONFIG:Debug>:${DEBUG_FLAGS}>"
)

add_link_options(
    "$<$<CONFIG:Debug>:-fsanitize=address>"
)

add_subdirectory(src)
