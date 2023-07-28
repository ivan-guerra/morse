cmake_minimum_required(VERSION 3.13...3.25)

project(translator
    DESCRIPTION "translate to and from morse code to audio"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE translator.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${MORSE_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    audio
)
