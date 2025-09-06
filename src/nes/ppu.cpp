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
    mmioWrite(Reg::PPUCTRL,    0b0000'0000);
    mmioWrite(Reg::PPUMASK,    0b0000'0000);
    mmioWrite(Reg::PPUSTATUS,  0b1010'0000);
    mmioWrite(Reg::OAMADDR,    0b0000'0000);
    // 2005/2006 latch?
    mmioWrite(Reg::PPUSCROLL,  0b0000'0000);
    mmioWrite(Reg::PPUADDR,    0b0000'0000);
    mmioWrite(Reg::PPUDATA,    0b0000'0000);
}


ubyte NesPPU::ioRead( uint16_t addr )
{

}


void NesPPU::ioWrite( uint16_t addr, ubyte value )
{

}

void NesPPU::tick( uint64_t dt )
{
    // printf("[PPU]\n");
}

