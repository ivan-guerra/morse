#!/bin/bash

CWD=$(pwd)

# Root directory.
MORSE_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
MORSE_SCRIPTS_PATH="${MORSE_PROJECT_PATH}/scripts"

# Binary directory.
MORSE_BIN_DIR="${MORSE_PROJECT_PATH}/bin"

# Resource directory.
MORSE_RES_DIR="${MORSE_PROJECT_PATH}/resources"

# CMake build files and cache.
MORSE_BUILD_DIR="${MORSE_PROJECT_PATH}/build"
