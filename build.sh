#!/bin/bash

# g++ --std=c++20 -lSDL2 main.cpp emu.cpp 6502/*.cpp system/*.cpp -o cpu

shopt -s globstar
g++ --std=c++20 -g -lSDL2 **/*.cpp -o cpu

