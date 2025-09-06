#pragma once
#include "file.hpp"
#include "../../hw/memory.hpp"
#include "../../hw/tmemory.hpp"


// https://www.nesdev.org/wiki/INES

/*
| Byte  | Description
|-------|-----------------------------------------------------------------------
| 0-3   | Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
| 4     | Size of PRG ROM in 16 KB units
| 5     | Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
| 6     | Flags 6 – Mapper, mirroring, battery, trainer
| 7     | Flags 7 – Mapper, VS/Playchoice, NES 2.0
| 8     | Flags 8 – PRG-RAM size (rarely used)
| 9     | Flags 9 – TV system (rarely used)
| 10    | Flags 10 – TV system, PRG-RAM presence (unofficial, rarely used)
| 11-15 | Unused (should be zero, some rippers leave name in bytes 7-15)
|-------------------------------------------------------------------------------
*/

/*
    An iNES file consists of the following sections, in order:
    - Header (16 bytes).
    - Trainer, if present (0 or 512 bytes).
    - PRG ROM data (16384 * x bytes).
    - CHR ROM data, if present (8192 * y bytes).
    - PlayChoice INST-ROM, if present (0 or 8192 bytes).
    - PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut). (this is often missing;
      see PC10 ROM-Images for details).
    - Some ROM-Images additionally contain a 128B (sometimes 127B) title at end of file.
*/


struct iNES_File
{
    char    signature[4];

    uint8_t prgRomSz; // size of PRG ROM in 16 KB units
    uint8_t chrRomSz; // Board uses CHR RAM if zero.

    struct {
        uint8_t f6MirroringMode    :1; // [0, 1] --> [hoz, vert] mirroring.
        uint8_t f6BatteryBackedRAM :1; // 1 for battery-backed RAM at $6000-$7FFF.
        uint8_t f6Trainer512byte   :1; // 1 for a 512-byte trainer at $7000-$71FF.
        uint8_t f6FourScreenVRAM   :1; // 1 for a four-screen VRAM layout.
        uint8_t MapperLo4          :4;
    } __attribute__((packed));

    struct {
        uint8_t f7VsUnisystem  :1; // 1 for VS-System GamePaks.
        uint8_t f7PlayChoice10 :1; // 8 KB of Hint Screen data after CHR
        uint8_t f7Flag8To15N20 :2; // If 0b10, flags 8-15 are in NES 2.0 format
        uint8_t MapperHi4      :4;
    } __attribute__((packed));
    
    struct {
        uint8_t prgRamSz; // Number of 8K banks, assume 1x8K if zero.
    } __attribute__((packed));

    struct {
        uint8_t f9IsPAL    :1; // assume 1x8kB RAM page if zero
        uint8_t f9Reserved :7;
    } __attribute__((packed));

    uint8_t flags10;    // Reserved, zero.
    uint8_t unused[5];  // Reserved, zero.

    uint8_t FileBody[];
};




/*
Byte    |   Contents
--------|-------------------------------------------------------------------
0-3     |   String "NES^Z" used to recognize .NES files.
        |
4       |   Number of 16kB ROM banks.
        |
5       |   Number of 8kB VROM banks.
        |
6       |   bit 0     1 for vertical mirroring, 0 for horizontal mirroring.
        |   bit 1     1 for battery-backed RAM at $6000-$7FFF.
        |   bit 2     1 for a 512-byte trainer at $7000-$71FF.
        |   bit 3     1 for a four-screen VRAM layout. 
        |   bit 4-7   Four lower bits of ROM Mapper Type.
        |
7       |   bit 0     1 for VS-System GamePaks.
        |   bit 1-3   Reserved, must be zeroes!
        |   bit 4-7   Four higher bits of ROM Mapper Type.
        |
8       |   Number of 8kB RAM banks. For compatibility with the previous
        |   versions of the .NES format, assume 1x8kB RAM page when this
        |   byte is zero.
        |
9       |   bit 0     1 for PAL GamePaks, otherwise assume NTSC.
        |   bit 1-7   Reserved, must be zeroes!
        |
10-15   |   Reserved, must be zeroes!
        |
16-...  |   ROM banks, in ascending order. If a trainer is present, its
        |   512 bytes precede the ROM bank contents.
        |
...-EOF |   VROM banks, in ascending order.
----------------------------------------------------------------------------
*/

