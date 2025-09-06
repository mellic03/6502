#include "device.hpp"
#include <cassert>


uint8_t HwDevice::busRead( uint16_t addr )
{
    if (!mBus) return 0;
    return mBus->read(addr);
}

void HwDevice::busWrite( uint16_t addr, uint8_t byte )
{
    if (!mBus) return;
    mBus->write(addr, byte);
}




ubyte *ioDevice::ioData()
{
    return mData;
}

ubyte ioDevice::ioRead( uint16_t addr )
{
    assert(addr < mSize);
    return mData[addr];
}

void ioDevice:: ioWrite( uint16_t addr, ubyte value )
{
    assert(addr < mSize);
    mData[addr] = value;
}

