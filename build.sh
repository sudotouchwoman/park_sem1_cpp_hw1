#!/usr/bin/bash
echo "Setting up the project"
TESTDIR=googletest/
[[ -d $TESTDIR ]] && echo "google test framework seems to be installed already" || git clone https://github.com/google/googletest.git
echo "Starts build"
mkdir -p build && cd build
cmake ..
cmake --build .
echo "Build finished"
cd ..
