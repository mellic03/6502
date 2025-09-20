#include <memu/hwmodule.hpp>
#include <memu/addrspace.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cstdlib>


memu::HwModule::HwModule( memu::AddrSpace &bus )
:   mBus(bus),
    mClock(0)
{
    mBus.attach(this);
}

uint8_t memu::HwModule::rdbus( uint16_t a )
{
    return mBus.read(a);
}

void memu::HwModule::wtbus( uint16_t a, uint8_t v )
{
    mBus.write(a, v);
}
