cmake_minimum_required(VERSION 3.13...3.25)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

project(audio
    DESCRIPTION "morse code WAV audio player"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE morse_code_audio_player.cc
    PRIVATE beeper.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${MORSE_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL2::Main
    PRIVATE SDL2::Mixer
)
