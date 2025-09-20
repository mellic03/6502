#pragma once

#include <memu/bus.hpp>
#include <memu/hwmodule.hpp>
#include <memu/memory.hpp>
#include <memu/rwx.hpp>


class NesPPU final: public memu::HwModule
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

    struct RegMMIO
    {
        struct {
            uint8_t NN :2; // X/Y Scroll
            uint8_t I  :1; // IncrementMode
            uint8_t S  :1; // SpriteTileSelect
            uint8_t B  :1; // BgTileSelect
            uint8_t H  :1; // SpriteHeight
            uint8_t P  :1; // MstrSlaveSelect
            uint8_t V  :1; // NMI enable
        }  PPUCTRL;

        struct {
            uint8_t G   :1; // greyscale
            uint8_t m   :1; // background left column enable
            uint8_t M   :1; // sprite left column enable
            uint8_t b   :1; // background enable
            uint8_t s   :1; // sprite enable
            uint8_t BGR :3; // colour emphasis
        } PPUMASK;

        struct {
            uint8_t U :5;  // unused
            uint8_t O :1;  // sprite overflow
            uint8_t S :1;  // sprite 0 hit
            uint8_t V :1;  // vblank
        } PPUSTATUS;

        uint8_t OAMADDR;    // OAM read/write address
        uint8_t OAMDATA;    // OAM read/write data
        uint8_t PPUSCROLL;  // 
        uint8_t PPUADDR;    // VRAM read/write address (two writes: MSB then LSB)
        uint8_t PPUDATA;    // VRAM read/write data
    };

    struct TableData
    {
        uint8_t nameTab[30][32];
        uint8_t attrTab[8][8];
    };


public:
    Memory2kRW mVRAM;
    RegMMIO   *mMMIO;
    TableData *mTables;

    NesPPU( memu::AddrSpace& );
    virtual void tick() final;
    virtual void reset() final;

};


/*
    https://www.nesdev.org/wiki/PPU_registers
    https://www.nesdev.org/wiki/PPU_memory_map
    https://www.nesdev.org/wiki/PPU_nametables
    https://www.nesdev.org/wiki/PPU_attribute_tables


    | Name      | Addr | Bits                | Type |
    |-----------------------------------------------|
    | PPUCTRL   | 2000 |           VPHB SINN | -W   | 
    | PPUMASK   | 2001 |           BGRs bMmG | -W   | 
    | PPUSTATUS | 2002 |           VSO- ---- | R-   | 
    | OAMADDR   | 2003 |           AAAA AAAA | -W   | 
    | OAMDATA   | 2004 |           DDDD DDDD | RW   | 
    | PPUSCROLL | 2005 | XXXX XXXX YYYY YYYY | Wx2  | 
    | PPUADDR   | 2006 | ..AA AAAA AAAA AAAA | Wx2  | 
    | PPUDATA   | 2007 |           DDDD DDDD | RW   | 
    | OAMDMA    | 4014 |           AAAA AAAA | -W   | 
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
    The NES has four logical nametables, arranged in a 2x2 pattern. Each occupies a 1 KiB
    chunk of PPU address space, starting at $2000 at the top left, $2400 at the top right,
    $2800 at the bottom left, and $2C00 at the bottom right. 

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

