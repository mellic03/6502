#include <memu/hwmodule.hpp>
#include <memu/addrspace.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cstdlib>


static void nofunc( void* )
{
    return;
}


memu::HwModule::HwModule( memu::AddrSpace &bus )
:   mBus(bus),
    mClock(0),
    mCallbackArg(nullptr),
    on_rdbus(nofunc),
    on_wtbus(nofunc)
{
    mBus.attach(this);
}

uint8_t memu::HwModule::rdbus( uint16_t a )
{
    on_rdbus(mCallbackArg);
    return mBus.read(a);
}

void memu::HwModule::wtbus( uint16_t a, uint8_t v )
{
    on_wtbus(mCallbackArg);
    mBus.write(a, v);
}
