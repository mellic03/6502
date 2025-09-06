#pragma once

#include "../hw/bus.hpp"
#include "../hw/device.hpp"
#include "../hw/clock.hpp"
#include "../hw/memory.hpp"
#include "../hw/mmio.hpp"
#include "../hw/tmemory.hpp"
#include "../rwx.hpp"


class NesPPU: public HwDevice
{
private:

     enum REG_: uint16_t
     {
          REG_PPUCTRL = 0x2000,
          REG_PPUMASK,
          REG_PPUSTATUS,
          REG_OAMADDR,
          REG_OAMDATA,
          REG_PPUSCROLL,
          REG_PPUADDR,
          REG_PPUDATA,
          REG_OAMDMA = 0x4014,
     };

     struct RegMMIO {
          uint8_t PPUCTRL, PPUMASK,   PPUSTATUS, OAMADDR;
          uint8_t OAMDATA, PPUSCROLL, PPUADDR,   PPUDATA;
     };

     struct NameTable {
          uint8_t data[30][32];
          uint8_t attr[8][8];
     };


public:
     Memory2kRW mVRAM;
     RegMMIO   *mMMIO;
     NameTable *mTables;

    NesPPU( DataBus *bus = nullptr );
    virtual void tick( uint64_t dt ) final;
};


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
        


    ---------------------------- Attribute Tables ----------------------------

    - An attribute table is a 64-byte array at the end of each nametable
      that controls which palette is assigned to each part of the background.

    - Each attribute table, starting at $23C0, $27C0, $2BC0, or $2FC0,
      is arranged as an 8x8 byte array:

          2xx0    2xx1    2xx2    2xx3    2xx4    2xx5    2xx6    2xx7
        ,-------+-------+-------+-------+-------+-------+-------+-------.
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xC0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xC8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xD0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xD8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xE0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xE8:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
   2xF0:| - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
        |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        +-------+-------+-------+-------+-------+-------+-------+-------+
   2xF8:|   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
        `-------+-------+-------+-------+-------+-------+-------+-------'



*/

