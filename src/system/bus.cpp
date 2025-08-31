#include "bus.hpp"
#include <stdio.h>


uint8_t BusInterface::rdbus( uint16_t addr )
{
    return mBus->mMem[deplex(addr)];
}

uint16_t BusInterface::rdbus16( uint16_t addr )
{
    return *(uint16_t*)(mBus->mMem + deplex(addr));
}

void BusInterface::wtbus( uint16_t addr, uint8_t byte )
{
    mBus->mMem[deplex(addr)] = byte;
}

void BusInterface::wtbus16( uint16_t addr, uint16_t word )
{
    *(uint16_t*)(mBus->mMem + deplex(addr)) = word;
}

