#include "bus.hpp"
#include <stdio.h>
#include <string.h>
#include <cassert>

DataBus::DataBus()
{
    memset(mPageTable, 0, sizeof(mPageTable));
    mPageFuncs.push_back({[](uint16_t){return 0;}, [](uint16_t, uint8_t){}});
    // mDevices.push_back({nullptr, 0, nullptr, nullptr});
}

void DataBus::tick()
{
    for (iBusDevice *dev: mDevices)
    {
        dev->Tick();
    }
}

void DataBus::map( uint16_t min, uint16_t max, ReadFunc rd, WriteFunc wt )
{
    uint16_t idx = mPageFuncs.size();
    mPageFuncs.push_back({rd, wt});

    for (uint32_t i=min; i<=max; i++)
    {
        mPageTable[(uint16_t)i] = idx;
    }
}

void DataBus::attach( iBusDevice *dev )
{
    mDevices.insert(dev);
}

uint8_t DataBus::read( uint16_t addr )
{
    return mPageFuncs[mPageTable[addr]].read(addr);
}

void DataBus::write( uint16_t addr, uint8_t byte )
{
    mPageFuncs[mPageTable[addr]].write(addr, byte);
}



#include <cassert>

uint8_t &MemoryDevice::operator[]( uint16_t i )
{
    assert((0<=i && i<=MaxAddr));
    return mMem[i];
}
