#pragma once

#include <memu/hw/mostech/6502.hpp>

namespace memu
{
    class Ricoh2A03;
}


class memu::Ricoh2A03: public m6502
{
public:
    Ricoh2A03(AddrSpace&);

private:
    union {
        uint8_t page[256];
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
        };
    } mApuMMIO;
};