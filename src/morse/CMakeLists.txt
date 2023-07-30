cmake_minimum_required(VERSION 3.13...3.25)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE morse.cc
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE io
    PRIVATE translator
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${MORSE_BIN_DIR}
)
