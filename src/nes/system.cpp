#include "system.hpp"
#include "cartridge.hpp"
#include "mapper/hwmapper.hpp"
#include <stdio.h>
#include <string.h>


NesEmu::System::System()
:   cpu_ram  (new uint8_t[2048]),
    cpu_rom  (new uint8_t[32*1024]),
    ppu_vram (new uint8_t[2048])
{
    cpu_bus.attach(&cpu);
    
    cpu_bus.map(0x0000, 0x1FFF,
        [=](uint16_t x) { return cpu_ram[x % 2048]; },
        [=](uint16_t x, uint8_t v) { cpu_ram[x % 2048] = v; }
    );
    
    cpu_bus.map(0x2000, 0x3FFF,
        [=](uint16_t x) { return ppu.mRegArray[x%8]; },
        [=](uint16_t x, uint8_t v) { ppu.mRegArray[x%8] = v; }
    );
    
    cpu_bus.map(0x4000, 0x4017,
        [=](uint16_t x) { return 0; },
        [=](uint16_t x, uint8_t v) {  }
    );

    // cpu_bus.map(0x4018, 0x401F, cpu_rd_apu, cpu_wt_apu);

    // [0x4020, 0xFFFF] --> Up to mapper.
    //     [0x6000, 0x7FFF] --> Usually cartridge RAM, when present. 
    //     [0x8000, 0xFFFF] --> Usually cartridge ROM and mapper registers.

    ppu_bus.attach(&ppu);

    ppu_bus.map(0x2000, 0x2FFF,
        [=](uint16_t x) { return ppu_vram[x % 2048]; },
        [=](uint16_t x, uint8_t v) { ppu_vram[x % 2048] = v; }
    );

    cpu.rdbus = [=](uint16_t x) { return cpu_bus.read(x); };
    cpu.wtbus = [=](uint16_t x, uint8_t v) { cpu_bus.write(x, v); };
}


void NesEmu::System::LoadRAW( uint8_t *rom )
{

}


void NesEmu::System::LoadROM( Cartridge *cart )
{
    mCartridge = cart;
    NesMem::getMapper(cart->mMapNo)->map(*this, cart);

    cpu.PC = ((uint16_t)cpu_bus.read(0xFFFD) << 8) | cpu_bus.read(0xFFFC);
    printf("Reset vector: 0x%04X\n", cpu.PC);
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


