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

    // CPU --> CPU wRAM,  mirror 2K banks until to 0x1FFF
    mBusCPU.mapRange(0x0000, 0x1FFF, 2048-1, cpuRAM);

    // CPU --> PPU mmio registers.
    mBusCPU.mapRange(0x2000, 0x4000, 8-1, ppuMMIO);

    // CPU --> NES APU and IO registers.
    mBusCPU.mapPage(0x4000, 32-1, apuMMIO);

    mBusPPU.attach(&mPPU);
    mBusPPU.mapPage(0x2000, 32-1, mAPU.data(), RWX::RW);
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

// void NesEmu::System::LoadROM( uint8_t *rom )
// {
//     INESHeader H = *(INESHeader*)rom;
//     uint16_t prgsize = 16384 * H.prgRomSize;
//     uint16_t chrsize = 8192  * H.chrRomSize;
//     memcpy(cpu_rom, rom, prgsize);

//     uint8_t mapno = (0b11110000 & H.flags6) >> 4
//                   + (0b11110000 & H.flags7) >> 4;

//     bool iNESFormat  = false;
//     bool NES20Format = false;

//     if (rom[0]=='N' && rom[1]=='E' && rom[2]=='S' && rom[3]==0x1A)
//     {
//         if (iNESFormat && (rom[7]&0x0C)==0x08)
//             NES20Format = true;
//         else
//             iNESFormat = true;
//     }

//     printf("signature   %s\n", H.signature);
//     printf("prgRomSize  %u\n", prgsize);
//     printf("chrRomSize  %u\n", chrsize);
//     printf("mapno       %u\n", mapno);
//     printf("flags6      %u\n", H.flags6);
//     printf("flags7      %u\n", H.flags7);
//     printf("flags8      %u\n", H.flags8);
//     printf("flags9      %u\n", H.flags9);
//     printf("flags10     %u\n", H.flags10);
//     printf("\n");

//     if (iNESFormat)
//     {
//         Mapper0(this);
//     }
    
//     if (NES20Format)
//     {
        
//     }

//     cpu.PC = ((uint16_t)cpu_bus.read(0xFFFD) << 8) | cpu_bus.read(0xFFFC);
//     printf("Reset vector: 0x%04X\n", cpu.PC);
// }


