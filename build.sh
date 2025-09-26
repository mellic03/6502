#!/bin/bash

mkdir -p ./build/CMake
cd ./build/CMake
cmake -DCMAKE_BUILD_TYPE=Debug ../../
make -j6
