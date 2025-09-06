#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"
#include "../hw/tmemory.hpp"
#include "../hw/clock.hpp"
#include "../rwx.hpp"
#include "mmio.hpp"



struct NesPPU_AttrTable
{
    uint8_t data[64];
};


union NesPPU_NameTable
{
    uint8_t data[1024];
    
    struct {
        uint8_t dat0[1024-64];
        uint8_t attr[64];
    };
};


// enum class NesPPU_Reg: uint8_t
// {
//     PPUCTRL, PPUMASK,   PPUSTATUS, OAMADDR,
//     OAMDATA, PPUSCROLL, PPUADDR,   PPUDATA,
//     NumValues
// };

// static constexpr
// uint8_t NesPPU_Acs[(uint8_t)NesPPU_Reg::NumValues] = {
//     RWX::W,  RWX::W,   RWX::R,   RWX::W,
//     RWX::RW, RWX::Wx2, RWX::Wx2, RWX::RW,
// };



class NesPPU: public HwDevice
{
private:
    uint8_t mPage[0x0100];

public:
    NesPPU();

    virtual uint8_t rd( uint16_t ) final { return 0; };
    virtual void wt( uint16_t, uint8_t ) final {  };
    virtual void tick() final;
};


// struct MMapPPU {
//     uint8_t PtrnTab[0x2000]; // 0x0000 -> 0x2000
//     uint8_t NameTab[0x1F00]; // 0x2000 -> 0x3F00
//     uint8_t Colours[0x0100]; // 0x3F00 -> 0x4000
// } *mMap;


/*
    https://www.nesdev.org/wiki/PPU_registers
    https://www.nesdev.org/wiki/PPU_memory_map
    https://www.nesdev.org/wiki/PPU_nametables
    https://www.nesdev.org/wiki/PPU_attribute_tables

    The NES has four logical nametables, arranged in a 2x2 pattern. Each occupies a 1 KiB
    chunk of PPU address space, starting at $2000 at the top left, $2400 at the top right,
    $2800 at the bottom left, and $2C00 at the bottom right. 
*/

/*
    |Name      | Addr  | Bits                | Type |
    |-----------------------------------------------|
    |PPUCTRL   | $2000 |           VPHB SINN | -W   | 
    |PPUMASK   | $2001 |           BGRs bMmG | -W   | 
    |PPUSTATUS | $2002 |           VSO- ---- | R-   | 
    |OAMADDR   | $2003 |           AAAA AAAA | -W   | 
    |OAMDATA   | $2004 |           DDDD DDDD | RW   | 
    |PPUSCROLL | $2005 | XXXX XXXX YYYY YYYY | Wx2  | 
    |PPUADDR   | $2006 | ..AA AAAA AAAA AAAA | Wx2  | 
    |PPUDATA   | $2007 |           DDDD DDDD | RW   | 
    |OAMDMA    | $4014 |           AAAA AAAA | -W   | 
    |-----------------------------------------------|

    PPUCTRL   - NMI enable (V), PPU master/slave (P), sprite height (H), background tile
                select (B), sprite tile select (S), increment mode (I), nametable select / X
                and Y scroll bit 8 (NN).
    PPUMASK   - Color emphasis (BGR), sprite enable (s), background enable (b), sprite left
                column enable (M), background left column enable (m), greyscale (G).
    PPUSTATUS - VBlank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair
                for $2005/$2006.
    OAMADDR   - OAM read/write address.
    OAMDATA   - OAM data read/write.
    PPUSCROLL - X and Y scroll bits 7-0 (two writes: X scroll, then Y scroll).
    PPUADDR   - VRAM address (two writes: most significant byte, then least significant byte).
    PPUDATA   - VRAM data read/write.
    OAMDMA    - OAM DMA high address.
        
*/


//        2xx0    2xx1    2xx2    2xx3    2xx4    2xx5    2xx6    2xx7
//      ,-------+-------+-------+-------+-------+-------+-------+-------.
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xC0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xC8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xD0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xD8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xE0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xE8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
// 2xF0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
//      |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      +-------+-------+-------+-------+-------+-------+-------+-------+
// 2xF8:|   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
//      `-------+-------+-------+-------+-------+-------+-------+-------'



