#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"
#include "../hw/tmemory.hpp"
#include "../hw/clock.hpp"
#include "../rwx.hpp"
#include "mmio.hpp"



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


enum class NesPPU_Reg: uint8_t
{
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA,
    NumValues
};

enum class NesPPU_Rwx: uint8_t
{
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


class NesPPU: public NesEmu::RegisterMMIO<NesPPU_Reg, NesPPU_Rwx>
{
private:
    using Reg = NesPPU_Reg;
    using Rwx = NesPPU_Rwx;

public:
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

    // NesEmu::RegisterMMIO<Reg, Rwx> mRegs;
    // uint8_t rd( Reg r ) { return mRegs.rd(r); };
    // void    wt( Reg r, uint8_t v ) { mRegs.wt(r, v); };

    virtual uint8_t rd( uint16_t ) final { return 0; };
    virtual void wt( uint16_t, uint8_t ) final {  };
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



