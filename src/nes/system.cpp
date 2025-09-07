#include "system.hpp"
#include "gamepak.hpp"
#include "mapper/mapper.hpp"
#include <stdio.h>
#include <string.h>

// hwtimer(1'790'000)

NesEmu::System::System()
:   mClock(1'790'000),
    mCPU(&mBusCPU),
    mPPU(&mBusPPU)
{
    ubyte *cpuRAM  = mCPU.mRAM.data();
    ubyte *ppuVRAM = mPPU.mVRAM.data();
    ubyte *ppuMMIO = (ubyte*)(mPPU.mMMIO);
    ubyte *apuMMIO = mAPU.data();

    // mBusCPU.attach(&mCPU);
    mBusCPU.mapRange(0x0000, 0x1FFF, 2048-1, cpuRAM);  // CPU --> CPU RAM, mirror every 2K
    mBusCPU.mapRange(0x2000, 0x3FFF, 0x08-1, ppuMMIO); // CPU --> PPU IO regs.
    mBusCPU.mapRange(0x4000, 0x401F, 0x001F, apuMMIO); // CPU --> NES APU and IO regs.

    mBusOTH.mapRdRange(0x2000, 0x3FFF, 8-1, ppuMMIO);

    // mBusPPU.attach(&mPPU);
    // mBusPPU.mapPage(0x2000, 32-1, mAPU.data(), RWX::RW);

}


void NesEmu::System::LoadRAW( uint8_t *rom )
{

}


void NesEmu::System::LoadROM( GamePak *gpak )
{
    mGPak = gpak;
    NesEmu::ExecuteMapper(gpak->mMapperNo, *this);

    mCPU.PC = (mBusCPU[0xFFFD] << 8) | mBusCPU[0xFFFC];
    printf("Reset vector: 0x%04X\n", mCPU.PC);
}


void NesEmu::System::tick( uint64_t dt )
{
    if (mClock.tick(dt))
    {
        mBusCPU.tick(dt/12);
        mBusPPU.tick(dt/4);
    }
}

