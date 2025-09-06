#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"


class NesAPU: public ioDevice
{
public:
    struct ApuMMIO {
        uint8_t  Pulse1[4];
        uint8_t  Pulse2[4];
        uint8_t  Triangle[3];
        uint8_t  Noise[6];
        uint8_t  DMC[4];
        uint8_t  Control;
        uint8_t  Status;
        uint8_t  FrameCounter;
        uint8_t  TestModeIO[8]; // Normally disabled
    } *mMMIO;

    NesAPU();
};
