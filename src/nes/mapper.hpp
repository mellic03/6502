#pragma once

#include <string.h>

#include "./ines.hpp"
#include "../system/bus.hpp"


struct MapperNROM
{
    void MapROM( iDataBus *bus, uint8_t *rom )
    {
        INESHeader H = *(INESHeader*)rom;
        uint16_t prgsize = 16384 * H.prgRomSize;
        uint16_t chrsize = 8192  * H.chrRomSize;

        bool iNESFormat=false;
        if (rom[0]=='N' && rom[1]=='E' && rom[2]=='S' && rom[3]==0x1A)
                iNESFormat=true;

        bool NES20Format=false;
        if (iNESFormat==true && (rom[7]&0x0C)==0x08)
                NES20Format=true;

        printf("signature   %s\n", H.signature);
        printf("prgRomSize  %u\n", prgsize);
        printf("chrRomSize  %u\n", chrsize);
        printf("flags6      %u\n", H.flags6);
        printf("flags7      %u\n", H.flags7);
        printf("flags8      %u\n", H.flags8);
        printf("flags9      %u\n", H.flags9);
        printf("flags10     %u\n", H.flags10);

        if (NES20Format)
        {
            
        }

        else if (iNESFormat)
        {
            memcpy(bus->mMem + 0xC000, rom+0x10, 16834-0x10);
            // printf("BOYE: 0x%04X\n", bus->mMem[0x6000 + 16834 - 2]);
        }
    }
};


