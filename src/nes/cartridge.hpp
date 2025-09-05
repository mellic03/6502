#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>



namespace NesEmu
{
    class Cartridge;
    class System;
    struct ROM_iNES;
}


class NesEmu::Cartridge
{
public:
    struct Fmt
    {
        enum { OTHER, iNES, NES20, NumFmt };
    };

    uint8_t mFormat;
    uint8_t _readfmt( uint8_t *rom );

    std::unique_ptr<uint8_t[]> mData = nullptr;
    uint8_t *mHead   = nullptr;
    uint8_t *mPrg    = nullptr;
    uint8_t *mChr    = nullptr;
    size_t   mDataSz = 0;
    size_t   mHeadSz = 0;
    size_t   mPrgSz  = 0;
    size_t   mChrSz  = 0;
    uint8_t  mMapNo  = 0;

    Cartridge( const std::string &path );
    bool is_bad() { return (mData.get() == nullptr); }

};



// https://www.nesdev.org/wiki/INES
/*
Byte    |   Contents
----------------------------------------------------------------------------
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
struct NesEmu::ROM_iNES
{
    char signature[4];

    uint8_t prgNum16kBanks; // size of PRG ROM in 16 KB units
    uint8_t chrNum8kBanks;  // Board uses CHR RAM if zero.

    union {
        uint8_t flags6;
        struct {
            uint8_t mirroringMode    :1; // [0, 1] --> [hoz, vert] mirroring.
            uint8_t batteryBackedRAM :1; // 1 for battery-backed RAM at $6000-$7FFF.
            uint8_t trainer512byte   :1; // 1 for a 512-byte trainer at $7000-$71FF.
            uint8_t fourScreenVRAM   :1; // 1 for a four-screen VRAM layout.
            uint8_t mapNo_lo         :4;
        } __attribute__((packed)) f6;
    };
    
    union {
        uint8_t flags7;
        struct {
            uint8_t vsSystemCatridge :1; // 1 for VS-System cartridges.
            uint8_t reserved         :3; // must be zero.
            uint8_t mapNo_hi         :4;
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
};
