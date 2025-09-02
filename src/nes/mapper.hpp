// #pragma once

// #include <string.h>

// #include "./ines.hpp"
// #include "../system/bus.hpp"

// /*
//     https://forums.nesdev.org/viewtopic.php?t=5661

//     The vast majority* of iNES ROM files are structured like this:
//     header, PRG ROM, CHR ROM
//     -or-
//     header, PRG ROM

//     - Read the first 16 bytes from the file. This is the header.
//     - Byte 0x0004 of the header is the PRG ROM size in 16384-byte units.
//     - Byte 0x0005 of the header is the CHR ROM size in 8192-byte units.
//       This may be 0, in which case the cartridge uses only CHR RAM.
//     - Now read the PRG ROM, and read the CHR ROM if it is present.
// */


// struct MapperNROM
// {
//     void MapROM( iDataBus *bus, uint8_t *rom )
//     {
//         INESHeader H = *(INESHeader*)rom;
//         uint16_t prgsize = 16384 * H.prgRomSize;
//         uint16_t chrsize = 8192  * H.chrRomSize;

//         bool iNESFormat=false;
//         if (rom[0]=='N' && rom[1]=='E' && rom[2]=='S' && rom[3]==0x1A)
//             iNESFormat=true;

//         bool NES20Format=false;
//         if (iNESFormat==true && (rom[7]&0x0C)==0x08)
//             NES20Format=true;

//         printf("signature   %s\n", H.signature);
//         printf("prgRomSize  %u\n", prgsize);
//         printf("chrRomSize  %u\n", chrsize);
//         printf("flags6      %u\n", H.flags6);
//         printf("flags7      %u\n", H.flags7);
//         printf("flags8      %u\n", H.flags8);
//         printf("flags9      %u\n", H.flags9);
//         printf("flags10     %u\n", H.flags10);
//         printf("\n");

//         if (NES20Format)
//         {
            
//         }

//         else if (iNESFormat)
//         {
//             memcpy(bus->mMem + 0x8000, rom+0x10, prgsize);
//             // for (size_t i=0x4000; i<0xFFFF; i+=prgsize)
//             // {
//             //     memcpy(bus->mMem + i, rom+0x10, prgsize);
//             // }
//             // memcpy(cpubus->mMem + 0x4000, rom+0x10, prgsize);
//             // memcpy(ppubus->mMem + 0x4000, rom+0x10, prgsize);
//             // printf("BOYE: 0x%04X\n", bus->mMem[0x6000 + 16834 - 2]);
//         }
//     }
// };


