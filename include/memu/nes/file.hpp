#pragma once
#include <memu/types.hpp>

namespace NesEmu
{
    struct iNES_File;
    struct NES20_File;
}



/*  iNES Header Contents
|-------------------------------------------------------------------------------
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
/*  iNES File Contents
|-------------------------------------------------------------------------------
| Byte    |   Contents
|---------|---------------------------------------------------------------------
| 0-3     |   String "NES^Z" used to recognize .NES files.
| 4       |   Number of 16kB ROM banks.
| 5       |   Number of 8kB VROM banks.
| 6       |   bit 0     [1,0] --> [vert/hoz mirroring].
|         |   bit 1     1 --> battery-backed RAM at $6000-$7FFF.
|         |   bit 2     1 --> 512-byte trainer at $7000-$71FF.
|         |   bit 3     1 --> four-screen VRAM layout. 
|         |   bit 4-7   MapNoLo4
| 7       |   bit 0     1 for VS-System GamePaks.
|         |   bit 1-3   Reserved, must be zeroes!
|         |   bit 4-7   MapNoHi4
| 8       |   Number of 8kB RAM banks. For compatibility with the previous
|         |   versions of the .NES format, assume 1x8kB RAM page when this
|         |   byte is zero.
| 9       |   bit 0     [0, 1] --> [NTSC, PAL]
|         |   bit 1-7   Reserved, must be zeroes!
| 10-15   |             Reserved, must be zeroes!
| 16-...  |   ROM banks, in ascending order. If a trainer is present, its
|         |   512 bytes precede the ROM bank contents.
| ...-EOF |   VROM banks, in ascending order.
|-------------------------------------------------------------------------------
*/
struct NesEmu::iNES_File
{
    char    Signature[4];
    uint8_t PrgRomNo16K; // size of PRG ROM in 16 KB units
    uint8_t ChrRomNo8K;  // Board uses CHR RAM if zero.
    struct {
        uint8_t MirroringMode    :1; // [0, 1] --> [hoz, vert] mirroring.
        uint8_t BatteryBackedRAM :1; // 1 for battery-backed RAM at $6000-$7FFF.
        uint8_t Trainer          :1; // 1 for a 512-byte trainer at $7000-$71FF.
        uint8_t FourScreenVRAM   :1; // 1 for a four-screen VRAM layout.
        uint8_t MapperLo4        :4;
    } __attribute__((packed));
    struct {
        uint8_t IsVSSystem   :1; // 1 for VS-System GamePaks.
        uint8_t PlayChoice10 :1; // 8 KB of Hint Screen data after CHR
        uint8_t Flag8To15N20 :2; // If 0b10, flags 8-15 are in NES 2.0 format
        uint8_t MapperHi4    :4;
    } __attribute__((packed));
    struct {
        uint8_t BankNo8K; // Number of 8K banks, assume 1x8K if zero.
    } __attribute__((packed));
    struct {
        uint8_t IsPAL    :1; // assume 1x8kB RAM page if zero
        uint8_t f9Reserved :7;
    } __attribute__((packed));
    uint8_t Flags10;    // Reserved, zero.
    uint8_t Unused[5];  // Reserved, zero.

    uint8_t contents[];
};



struct NesEmu::NES20_File
{
    uint8_t header[16];
    uint8_t contents[];
};

