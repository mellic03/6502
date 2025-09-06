#include "device.hpp"
#include <cassert>


ubyte HwDevice::busRead( uint16_t i )
{
    if (!mBus) return 0;
    return mBus->read(i);
}

void HwDevice::busWrite( uint16_t i, ubyte v )
{
    if (!mBus) return;
    mBus->write(i, v);
}



ubyte &ioDevice::operator[]( uint16_t i )
{
    assert(i < mSize);
    return mData[i];
}


ubyte ioDevice::ioRead( uint16_t i )
{
    assert(i < mSize);
    return mData[i];
}

void ioDevice:: ioWrite( uint16_t i, ubyte v )
{
    assert(i < mSize);
    mData[i] = v;
}

