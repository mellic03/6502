#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"
#include "../hw/tmemory.hpp"
#include "../hw/clock.hpp"
#include "mmio.hpp"
#include "rwx.hpp"



// PPUCTRL 	    $2000 	VPHB SINN 	W 	NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B), sprite tile select (S), increment mode (I), nametable select / X and Y scroll bit 8 (NN)
// PPUMASK 	    $2001 	BGRs bMmG 	W 	color emphasis (BGR), sprite enable (s), background enable (b), sprite left column enable (M), background left column enable (m), greyscale (G)
// PPUSTATUS    $2002 	VSO- ---- 	R 	vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
// OAMADDR 	    $2003 	AAAA AAAA 	W 	OAM read/write address
// OAMDATA 	    $2004 	DDDD DDDD 	RW 	OAM data read/write
// PPUSCROLL 	$2005 	XXXX XXXX YYYY YYYY 	Wx2 	X and Y scroll bits 7-0 (two writes: X scroll, then Y scroll)
// PPUADDR 	    $2006 	..AA AAAA AAAA AAAA 	Wx2 	VRAM address (two writes: most significant byte, then least significant byte)
// PPUDATA 	    $2007 	DDDD DDDD 	RW 	VRAM data read/write
// OAMDMA 	    $4014 	


// class DataBusPPU: public iDataBus
// {
// private:
// public:
//     struct MMapPPU {
//         uint8_t vrom[0x2000]; // 0x0000 -> 0x2000
//         uint8_t vram[0x1F00]; // 0x2000 -> 0x3F00
//         uint8_t data[0x0100]; // 0x3F00 -> 0x4000
//     };

//     DataBusPPU(): iDataBus(new uint8_t[0x4000]) {  }
// };

// class BusInterfacePPU: public BusInterface
// {
// private:
//     uint16_t deplex( uint16_t addr )
//     {
//         return addr & 0x7FFF; // 0x7FFE&0x7FFF; 0x7FFF&0x7FFF; 0x8000&0x7FFF; 0x8001&0x7FFF;
//     }

// public:
//     BusInterfacePPU( iDataBus *bus ): BusInterface(bus) {  }

// };


template <size_t mNumRegs>
struct NesMMIO
{
    struct mmio_entry_t { uint8_t val, rwx; };
    mmio_entry_t mRegs[mNumRegs];

    void wt( uint8_t idx, uint8_t val )
    {
        if (mRegs[idx].rwx & RWX::W)
            mRegs[idx].val = val;
    }
};


class NesPPU: public ioDevice
{
private:
    enum class regPPU: uint8_t {
        REG_PPUCTRL,
        REG_PPUMASK,
        REG_PPUSTATUS,
        REG_OAMADDR,
        REG_OAMDATA,
        REG_PPUSCROLL,
        REG_PPUADDR,
        REG_PPUDATA,
        NumValues
    };

    enum class rwxPPU: uint8_t {
        RWX_PPUCTRL   = RWX::W,
        RWX_PPUMASK   = RWX::W,
        RWX_PPUSTATUS = RWX::R,
        RWX_OAMADDR   = RWX::W,
        RWX_OAMDATA   = RWX::RW,
        RWX_PPUSCROLL = RWX::Wx2,
        RWX_PPUADDR   = RWX::Wx2,
        RWX_PPUDATA   = RWX::RW,
        NumValues     = 8
    };

public:
    struct mmio_t 
    {
        uint8_t PPUCTRL;
        uint8_t PPUMASK;
        uint8_t PPUSTATUS;
        uint8_t OAMADDR;
        uint8_t OAMDATA;
        uint8_t PPUSCROLL;
        uint8_t PPUADDR;
        uint8_t PPUDATA;
        // uint8_t OAMDMA; 
    };

    struct AttrTable
    {
        uint8_t data[64];
    };
    
    union NameTable
    {
        uint8_t data[1024];
        
        struct {
            uint8_t dat0[1024-64];
            uint8_t attr[64];
        };
    };

    
    static constexpr uint8_t REG_R   = 0b00'01;
    static constexpr uint8_t REG_W   = 0b00'10;
    static constexpr uint8_t REG_RW  = 0b00'11;

    static constexpr uint8_t REG_x2  = 0b01'00;
    static constexpr uint8_t REG_Rx2 = REG_R  | REG_x2;
    static constexpr uint8_t REG_Wx2 = REG_W  | REG_x2;

    union
    {
        uint8_t mRegArray[8];

        struct
        {
            uint8_t PPUCTRL;
            uint8_t PPUMASK;
            uint8_t PPUSTATUS;
            uint8_t OAMADDR;
            uint8_t OAMDATA;
            uint8_t PPUSCROLL;
            uint8_t PPUADDR;
            uint8_t PPUDATA;
        };
    };

    const uint8_t mRegAccess[8] = {
        REG_W,  REG_W,   REG_R,   REG_W,
        REG_RW, REG_Wx2, REG_Wx2, REG_RW,
    };


    NesPPU();
    virtual uint8_t rd( uint16_t ) final;
    virtual void wt( uint16_t, uint8_t ) final;
    virtual void Tick() final;
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



