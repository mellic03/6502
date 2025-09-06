#include "ppu.hpp"
#include <stdio.h>


/*
    Register	        At Power    After Reset
    PPUCTRL             0000 0000	0000 0000
    PPUMASK             0000 0000	0000 0000
    PPUSTATUS           +0+x xxxx	U??x xxxx
    OAMADDR             $00	        unchanged
    $2005/$2006 latch	cleared     cleared
    PPUSCROLL           $0000	    $0000
    PPUADDR             $0000	    unchanged
    PPUDATA             read buffer	$00	$00

    ? = unknown, x = irrelevant, + = often set, U = unchanged
*/
NesPPU::NesPPU()
:   mTables(new NameTable[4])
{
    mmio_write(Reg::PPUCTRL,    0b0000'0000);
    mmio_write(Reg::PPUMASK,    0b0000'0000);
    mmio_write(Reg::PPUSTATUS,  0b1010'0000);
    mmio_write(Reg::OAMADDR,    0b0000'0000);
    // 2005/2006 latch?
    mmio_write(Reg::PPUSCROLL,  0b0000'0000);
    mmio_write(Reg::PPUADDR,    0b0000'0000);
    mmio_write(Reg::PPUDATA,    0b0000'0000);
}


void NesPPU::tick( uint64_t dt )
{
    // printf("[PPU]\n");
}

