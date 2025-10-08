#!/bin/bash

mkdir -p ./build/CMake
cd ./build/CMake
cmake -DCMAKE_BUILD_TYPE=Release ../../
make -j6
