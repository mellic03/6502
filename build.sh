#!/bin/bash

mkdir -p ./build

cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
make -j6
mv ./nesemu ../nesemu
