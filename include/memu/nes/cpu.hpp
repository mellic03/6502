#pragma once
#include <memu/hw/mos6502.hpp>


class Ricoh2A03: public m6502
{
public:
    using m6502::m6502;

private:
    struct apu_mmio_t {
        uint8_t  Pulse1[4];
        uint8_t  Pulse2[4];
        uint8_t  Triangle[3];
        uint8_t  Noise[6];
        uint8_t  DMC[4];
        uint8_t  Control;
        uint8_t  Status;
        uint8_t  FrameCounter;
        uint8_t  TestModeIO[8]; // Normally disabled
    } mApuMMIO;

};


using NesCPU = Ricoh2A03;

