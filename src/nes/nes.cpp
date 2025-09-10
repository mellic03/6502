#include <memu/nes/nes.hpp>
#include <memu/nes/mapper.hpp>
#include <stdio.h>
#include <string.h>

NesEmu::System::System()
// :   mClock(1'790'000),
:   mCPU(mBusCPU),
    mPPU(mBusPPU),
    mGPak(nullptr)
{
    ubyte *cpuRAM  = mCPU.mRAM.data();
    ubyte *ppuVRAM = mPPU.mVRAM.data();
    ubyte *ppuMMIO = (ubyte*)(mPPU.mMMIO);
    // ubyte *apuMMIO = mAPU.data();

    // mBusCPU.attach(&mCPU);
    mBusCPU.mapRange(0x0000, 0x1FFF, 2048-1, memu::RWX_RW, cpuRAM);  // CPU --> CPU RAM, mirror every 2K
    // mBusCPU.mapRdWtRange(0x4000, 0x401F, 0x001F, apuMMIO); // CPU --> NES APU and IO regs.

    // mBusPPU.attach(&mPPU);
    // mBusPPU.mapPage(0x2000, 32-1, mAPU.data(), RWX::RW);
}


void NesEmu::System::LoadROM( GamePak *gpak )
{
    mGPak = gpak;
    Mapper::MapGamePak(*this, gpak);

    mCPU.PC = (mBusCPU.read(0xFFFD) << 8) | mBusCPU.read(0xFFFC);
    printf("Reset vector: 0x%04X\n", mCPU.PC);
}


void NesEmu::System::tick()
{
    // if (mClock.tick(0))
    {
        mBusCPU.tick();
        mBusPPU.tick();
    }
}

