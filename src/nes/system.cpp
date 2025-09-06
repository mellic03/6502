#include "system.hpp"
#include "gamepak.hpp"
#include "mapper/hwmapper.hpp"
#include <stdio.h>
#include <string.h>

// hwtimer(1'790'000)

NesEmu::System::System()
:   mClock(1'790'000)
{
    ubyte *cpuRAM  = mCPU.mRAM.data<ubyte>();
    ubyte *ppuRAM  = mPPU.mRAM.data<ubyte>();
    ubyte *ppuMMIO = mPPU.mMMIO.data<ubyte>();
    ubyte *apuMMIO = mAPU.data<ubyte>();

    mBusCPU.attach(&mCPU);
    mBusCPU.mapRange(0x0000, 0x1FFF, 2048-1, cpuRAM);  // CPU --> CPU wRAM,  mirror 2K banks until to 0x1FFF
    mBusCPU.mapRange(0x2000, 0x3FFF, 0x08-1, ppuMMIO); // CPU --> PPU IO regs.
    mBusCPU.mapPage(0x4000, 32-1, apuMMIO);            // CPU --> NES APU and IO regs.

    mBusPPU.attach(&mPPU);
    // mBusPPU.mapPage(0x2000, 32-1, mAPU.data(), RWX::RW);
}


void NesEmu::System::LoadRAW( uint8_t *rom )
{

}


void NesEmu::System::LoadROM( GamePak *gpak )
{
    mGPak = gpak;
    NesEmu::callHwMapper(gpak->mMapperNo, *this);

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

