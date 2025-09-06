#pragma once

#include "../6502/6502.hpp"
#include "../hw/bus.hpp"
#include "../hw/clock.hpp"
#include "../hw/tmemory.hpp"
#include "./ppu.hpp"
#include "./apu.hpp"



namespace NesEmu
{
    class GamePak;
    class System;
}


#define DC_FUNC(Content) \
    [](uint16_t addr) -> uint16_t { Content }

#define RD_FUNC(Tp, Content) \
    [](HwDevice *ibd, uint16_t addr) -> uint8_t \
    {\
        Tp *dev = static_cast<Tp*>(ibd);\
        Content\
    }

#define WT_FUNC(Tp, Content) \
    [](HwDevice *ibd, uint16_t addr, uint8_t byte) \
    {\
        Tp *dev = static_cast<Tp*>(ibd);\
        Content\
    }


class NesEmu::System
{
public:
    HwClock  mClock;
    // Memory2kRW  wRAM;
    // Memory2kRW  vRAM;

    DataBus  mBusCPU;
    DataBus  mBusPPU;
    cpu6502  mCPU;
    NesPPU   mPPU;
    NesAPU   mAPU;
    GamePak *mGPak;

    System();
    void LoadRAW( uint8_t *rom );
    void LoadROM( GamePak* );
    void tick( uint64_t dt );

};


