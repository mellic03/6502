#include "ppu.hpp"
#include <stdio.h>


NesPPU::NesPPU()
{

}


uint8_t NesPPU::rd( uint16_t addr )
{
    uint16_t idx = addr % 8;
    if (mRegAccess[idx] & REG_R)
        return mRegArray[idx];
    return 0;
}


void NesPPU::wt( uint16_t addr, uint8_t byte )
{
    uint16_t idx = addr % 8;
    if (mRegAccess[idx] & REG_W)
        mRegArray[idx] = byte;
}




void NesPPU::Tick()
{
    // printf("[PPU]\n");
}

