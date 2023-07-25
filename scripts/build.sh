#!/bin/bash

BUILD_TYPE="Release"

source config.sh

Help()
{
    echo "build the morse project"
    echo
    echo "usage: build.sh [OPTION]..."
    echo "options:"
    echo -e "\tg    enable debug info"
    echo -e "\th    print this help message"
}

Main()
{
    # Create the build directory if it does not already exist.
    mkdir -pv $MORSE_BUILD_DIR

    pushd $MORSE_BUILD_DIR > /dev/null
        cmake ../ \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
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

while getopts ":hg" flag
do
    case "$flag" in
        g) BUILD_TYPE="Debug";;
        h) Help
           exit;;
       \?) echo "error: invalid option '$OPTARG'"
           Help
           exit;;
    esac
done

Main
