#include "device.hpp"
#include <cassert>



HwDevice::HwDevice( DataBus *bus=nullptr )
:   mBus(bus)
{
    if (mBus)
    {
        mBus->attach(this);
    }
}


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
    return mBytes[i];
}


ubyte ioDevice::ioRead( uint16_t i )
{
    assert(i < mSize);
    return mBytes[i];
}

void ioDevice:: ioWrite( uint16_t i, ubyte v )
{
    assert(i < mSize);
    mBytes[i] = v;
}

