#pragma once

#include "../gamepak.hpp"
#include "../system.hpp"

namespace NesEmu
{
    void ExecuteMapper( int mapno, System& );
}

void Mapper00_NROM(NesEmu::System&);
void Mapper01_MMC1(NesEmu::System&);
void Mapper02_UxROM(NesEmu::System&);
void Mapper03_CNROM(NesEmu::System&);
void Mapper04_MMC3(NesEmu::System&);



struct HwMapper
{
    struct PageDesc {
        ubyte *page;
        uword  base;
        uword  mask;
        ubyte  rwx; // 0b0RWX -> 0b0111
    } mPageTable[256];

    ubyte *getPtr( uint16_t addr, uint8_t access )
    {
        ubyte page_idx = (addr & 0x00FF);
        auto &[page, base, mask, rwx] = mPageTable[page_idx];

        if (page && (rwx & access))
        {
            return &page[(addr - base) & mask];
        }
    
        return nullptr;
    }

    ubyte pageRead( uint16_t addr )
    {
        ubyte *ptr = getPtr(addr, RWX::R);
        return (ptr) ? *ptr : 0;
    }

    void pageWrite( uint16_t addr, uint8_t value )
    {
        if (ubyte *ptr = getPtr(addr, RWX::W))
        {
            *ptr = value;
        }
    }

};



