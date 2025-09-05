#include "ppu.hpp"
#include <stdio.h>



BusDevicePPU::BusDevicePPU()
:   mRam(2048)
{
    // mBus.attach(&mRam, 0x2000, 0x3FFF);
    // mBus.attach(&mRom);
    // mMap = (MMapPPU*)(&mBus[0]);
    // mMap->PtrnTab
}


void BusDevicePPU::Tick()
{
    // printf("[PPU]\n");
}

