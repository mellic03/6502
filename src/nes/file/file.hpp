#pragma once

#include <cstddef>
#include <cstdint>


namespace NesFile
{
    struct Base;
}


struct NesFile::Base
{
public:
    uint8_t *mData;
    size_t   mSize;

    Base( uint8_t *raw = nullptr, size_t size = 0)
    :   mData(raw), mSize(size) {  }
};



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
        uint8_t chrRamSz; // No. 8K banks, assume 1x8K if zero.
    } __attribute__((packed));

    struct {
        uint8_t f9IsPAL    :1; // assume 1x8kB RAM page if zero
        uint8_t f9Reserved :7;
    } __attribute__((packed));

    uint8_t flags10;    // Reserved, zero.
    uint8_t unused[5];  // Reserved, zero.

    uint8_t FileBody[];
};


