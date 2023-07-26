cmake_minimum_required(VERSION 3.13...3.25)

# This CMakeLists.txt was taken from here
# https://vicrucann.github.io/tutorials/quick-cmake-doxygen/

option(BUILD_DOCS "build morse docs" OFF)

# Check if Doxygen is installed.
find_package(Doxygen)
if (DOXYGEN_FOUND)
    if (BUILD_DOCS)
        # Set input and output files.
        set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
        set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

        # Request to configure the file.
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("doxygen build started")

        # Note the option ALL which allows to build the docs together with the
        # application.
        add_custom_target(Docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "generating API documentation with Doxygen"
            VERBATIM
        )
    else (BUILD_DOCS)
        message("BUILD_DOCS=OFF, project documentation will not be built")
    endif (BUILD_DOCS)
else (DOXYGEN_FOUND)
  message("please install Doxygen to build the documentation")
endif (DOXYGEN_FOUND)
