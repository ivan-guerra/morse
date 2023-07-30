#!/bin/bash

BUILD_DOCS="OFF"
BUILD_TESTS="OFF"
BUILD_TYPE="Release"

source config.sh

Help()
{
    echo "build the morse project"
    echo
    echo "usage: build.sh [OPTION]..."
    echo "options:"
    echo -e "\tg    enable debug info"
    echo -e "\td    build project docs"
    echo -e "\tt    build unit tests"
    echo -e "\th    print this help message"
}

Main()
{
    # Create the build directory if it does not already exist.
    mkdir -pv $MORSE_BUILD_DIR

    pushd $MORSE_BUILD_DIR > /dev/null
        cmake ../ \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
              -DBUILD_DOCS=$BUILD_DOCS \
              -DPACKAGE_TESTS=$BUILD_TESTS \
              -DCMAKE_INSTALL_PREFIX=$MORSE_BIN_DIR \
              -DCMAKE_BUILD_TYPE=$BUILD_TYPE && \
        make -j$(nproc) all                  && \
        make install

        # Exit if any of the above commands fails.
        if [ $? -ne 0 ];
        then
            exit 1
        fi
    popd > /dev/null
}

while getopts ":hgdt" flag
do
    case "$flag" in
        g) BUILD_TYPE="Debug";;
        d) BUILD_DOCS="ON";;
        t) BUILD_TESTS="ON";;
        h) Help
           exit;;
       \?) echo "error: invalid option '$OPTARG'"
           Help
           exit;;
    esac
done

Main
