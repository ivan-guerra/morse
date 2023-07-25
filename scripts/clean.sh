#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d $MORSE_BIN_DIR ]
then
    echo "removing '$MORSE_BIN_DIR'"
    rm -r $MORSE_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $MORSE_BUILD_DIR ]
then
    echo "removing '$MORSE_BUILD_DIR'"
    rm -r $MORSE_BUILD_DIR
fi
