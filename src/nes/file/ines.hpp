#pragma once
#include "file.hpp"

namespace NesFile
{
    struct iNES;
    struct iNES_Info;
}


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
7       |   bit 0     1 for VS-System cartridges.
        |   bit 1-3   Reserved, must be zeroes!
        |   bit 4-7   Four higher bits of ROM Mapper Type.
        |
8       |   Number of 8kB RAM banks. For compatibility with the previous
        |   versions of the .NES format, assume 1x8kB RAM page when this
        |   byte is zero.
        |
9       |   bit 0     1 for PAL cartridges, otherwise assume NTSC.
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
struct NesFile::iNES: public NesFile::File
{
    struct FileHeader {
        char    signature[4];
        uint8_t prgNo16kBanks; // size of PRG ROM in 16 KB units
        uint8_t chrNo8kBanks;  // Board uses CHR RAM if zero.

        union {
            uint8_t flags6;
            struct {
                uint8_t mirroringMode    :1; // [0, 1] --> [hoz, vert] mirroring.
                uint8_t batteryBackedRAM :1; // 1 for battery-backed RAM at $6000-$7FFF.
                uint8_t trainer512byte   :1; // 1 for a 512-byte trainer at $7000-$71FF.
                uint8_t fourScreenVRAM   :1; // 1 for a four-screen VRAM layout.
                uint8_t mapNo_lo4        :4;
            } __attribute__((packed)) f6;
        };
        
        union {
            uint8_t flags7;
            struct {
                uint8_t vsUnisystem  :1; // 1 for VS-System cartridges.
                uint8_t playChoice10 :1; // 8 KB of Hint Screen data after CHR
                uint8_t flag8To15N20 :2; // If 0b10, flags 8-15 are in NES 2.0 format
                uint8_t mapNo_hi4    :4;
            } __attribute__((packed)) f7;
        };
        
        union {
            uint8_t flags8;
            struct {
                uint8_t ramNum8kBanks; // assume 1x8kB RAM page if zero
            } __attribute__((packed)) f8;
        };

        union {
            // Bit 0 only. Bits 1-7 must be all zero.
            uint8_t flags9;
            struct {
                uint8_t isPAL    :1; // assume 1x8kB RAM page if zero
                uint8_t reserved :7;
            } __attribute__((packed)) f9;
        };

        uint8_t flags10;    // Reserved, zero.
        uint8_t unused[5];  // Reserved, zero.
    } __attribute__((packed));


    /**
     * An iNES file consists of the following sections, in order:
     * - Header (16 bytes)
     * - Trainer, if present (0 or 512 bytes)
     * - PRG ROM data (16384 * x bytes)
     * - CHR ROM data, if present (8192 * y bytes)
     * - PlayChoice INST-ROM, if present (0 or 8192 bytes)
     * - PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut)
     *   (this is often missing; see PC10 ROM-Images for details).
     * - Some ROM-Images additionally contain a 128-byte (or sometimes 127-byte)
     *   title at the end of the file.
     */
    // struct FileInfo {
        uint8_t  mapperNo;

        uint64_t headerSz = 16;
        uint64_t trainerSz;
        uint64_t prgRomSz;
        uint64_t chrRomSz;
        uint64_t playChoiceInstRomSz;
        uint64_t playChoicePRomSz;
        uint64_t titleSz;
    // } __attribute__((packed));
    // FileInfo info;

    uint8_t *data;

    iNES( uint8_t *base );
};

// struct NesFile_iNES
// {
//     uint8_t header[16];
//     uint8_t prgRom[0x4000];
//     uint8_t chrRom[0x2000];
//     uint8_t data[];
// };


