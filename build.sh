#!/bin/bash

# shopt -s globstar
# g++ --std=c++17 **/*.cpp -o nesemu -lSDL2

mkdir -p ./build

cd ./build
cmake ..
make -j6
mv ./nesemu ../nesemu
