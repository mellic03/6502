#include "ppu.hpp"
#include <stdio.h>



BusAttachmentPPU::BusAttachmentPPU( iDataBus *bus )
:   BusAttachment(bus),
    SignalListener(2)
{

}


void BusAttachmentPPU::Tick()
{
    printf("[PPU]\n");
}

