#include "system.hpp"
#include "gamepak.hpp"
#include "mapper/hwmapper.hpp"
#include <stdio.h>
#include <string.h>

// hwtimer(1'790'000)

NesEmu::System::System()
{
    static constexpr auto AAA = 0x4000/256;
    static constexpr auto CCC = 0x4017/256;

    static constexpr auto DAA = 0x4017;
    static constexpr auto DCC = 0x2000/256;

    using iBD = iBusDevice;
    using Sys = System;
    using u08 = uint8_t;
    using u16 = uint16_t;

    mBusCPU.attach(&mCPU);

    mBusCPU.map(&wRAM, 0x0000, 0x1FFF, // CPU --> CPU working RAM.
        DC_FUNC(         return addr % 2048;    ),
        RD_FUNC(MemRW2K, return dev->rd(addr);  ),
        WT_FUNC(MemRW2K, dev->wt(addr, byte);   )
    );

    mBusCPU.map(&mPPU, 0x2000, 0x3FFF, // CPU --> PPU mmio registers.
        DC_FUNC(        return 0x2000 + (addr % 8); ),
        RD_FUNC(NesPPU, return dev->rd(addr);  ),
        WT_FUNC(NesPPU, dev->wt(addr, byte);  )
    );

    mBusCPU.map(&mAPU, 0x4000, 0x4017, // CPU --> NES APU and IO registers.
        DC_FUNC(        return 0x4000 + (addr % 0x18);  ),
        RD_FUNC(NesAPU, return dev->mTestMem[addr];     ),
        WT_FUNC(NesAPU, dev->mTestMem[addr] = byte;     )
    );

    // [0x4020, 0xFFFF] --> Up to mapper.
    //     [0x6000, 0x7FFF] --> Usually GamePak RAM, when present. 
    //     [0x8000, 0xFFFF] --> Usually GamePak ROM and mapper registers.

    mBusPPU.attach(&mPPU);

    // mBusPPU.map(&XXXXXX, 0x2000, 0x2FFF, // PPU --> XXXXXX
    //     DC_FUNC(        return 0x2000 + (addr % 0x08);  ),
    //     RD_FUNC(XXXXXX, return 0;       ),
    //     WT_FUNC(XXXXXX, byte = byte;    )
    // );

    mCPU.rdbus = [=](uint16_t x) { return mBusCPU.read(x); };
    mCPU.wtbus = [=](uint16_t x, uint8_t v) { mBusCPU.write(x, v); };

}


void NesEmu::System::LoadRAW( uint8_t *rom )
{

}


void NesEmu::System::LoadROM( GamePak *cart )
{
    mGamePak = cart;
    NesEmu::getMapper(cart->miNES->mInfo.mapperNo)->map(*this, cart);

    mCPU.PC = ((uint16_t)mBusCPU.read(0xFFFD) << 8) | mBusCPU.read(0xFFFC);
    printf("Reset vector: 0x%04X\n", mCPU.PC);
}



void NesEmu::System::Tick()
{
    mBusCPU.tick();

    mBusPPU.tick();
    mBusPPU.tick();
    mBusPPU.tick();
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


