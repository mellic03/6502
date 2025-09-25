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


ubyte memu::HwModule::rdbus( addr_t addr )
{
    on_rdbus(mCallbackArg);
    return mBus.read(addr);
}

uword memu::HwModule::rdbusw( addr_t addr )
{
    uint16_t lo = (uint16_t)rdbus(addr+0);
    uint16_t hi = (uint16_t)rdbus(addr+1);
    return (hi << 8) | lo;
}

void memu::HwModule::wtbus( addr_t addr, ubyte v )
{
    on_wtbus(mCallbackArg);
    mBus.write(addr, v);
}
