#include "ppu.hpp"
#include <stdio.h>



BusAttachmentPPU::BusAttachmentPPU( iDataBus *bus )
:   BusAttachment(bus),
    SignalListener(2)
{
    // mMap = (MMapPPU*)(&mBus[0]);
    // mMap->PtrnTab
}


void BusAttachmentPPU::Tick()
{
    // printf("[PPU]\n");
}

