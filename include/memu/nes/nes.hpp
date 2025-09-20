#pragma once

#include <memu/addrspace.hpp>
#include "cpu.hpp"
#include "ppu.hpp"


namespace NesEmu
{
    class GamePak;
    class System;
}


class NesEmu::System
{
public:
    System(const System&) = delete;
    System &operator=(const System&) = delete;
    // HwClock  mClock;
    // Memory2kRW  wRAM;
    // Memory2kRW  vRAM;

    memu::AddrSpace mBusCPU;
    memu::AddrSpace mBusPPU;
    NesCPU   mCPU;
    NesPPU   mPPU;
    GamePak *mGPak;

    System();
    void loadGamePak( GamePak* );
    void tick();

};

