#pragma once

#include "../6502/6502.hpp"
#include "../hw/bus.hpp"
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
    [](iBusDevice *ibd, uint16_t addr) -> uint8_t \
    {\
        Tp *dev = static_cast<Tp*>(ibd);\
        Content\
    }

#define WT_FUNC(Tp, Content) \
    [](iBusDevice *ibd, uint16_t addr, uint8_t byte) \
    {\
        Tp *dev = static_cast<Tp*>(ibd);\
        Content\
    }


class NesEmu::System: public iBusDevice
{
public:
    MemRW2K  wRAM;
    MemRW2K  vRAM;
    DataBus  mBusCPU;
    DataBus  mBusPPU;
    cpu6502  mCPU;
    NesPPU   mPPU;
    NesAPU   mAPU;
    GamePak *mGamePak;

    System();
    void LoadRAW( uint8_t *rom );
    void LoadROM( GamePak* );
    virtual void Tick() final;

};


