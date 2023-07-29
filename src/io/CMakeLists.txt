cmake_minimum_required(VERSION 3.13...3.25)

project(io
    DESCRIPTION "io library for reading and writing lists of words"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE io.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${MORSE_INCLUDE_DIR}
)
