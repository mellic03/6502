#pragma once

#include <string.h>

#include "./ines.hpp"
#include "../system/bus.hpp"


struct MapperNROM
{
    void MapROM( iDataBus *bus, uint8_t *rom )
    {
        INESHeader H = *(INESHeader*)rom;
        uint16_t size = 16384 * H.prgRomSize;

        memcpy(bus->mMem + 0xC000, rom+0x0010, 16834);

        if (size == 2*16384)
        {
            memcpy(bus->mMem + 0xC000, rom, 16834);
        }
    }
};


