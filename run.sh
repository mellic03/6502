#!/bin/bash

# ./nesemu --rom ./bin/nestest.nes --jump 0xC000

cp -R ./data/* ./build/NesEmu/
cd ./build/NesEmu
./nesemu --conf ./profile/default.conf
# ./test_m6502
