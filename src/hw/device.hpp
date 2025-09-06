#pragma once

#include "bus.hpp"
using ubyte = uint8_t;

class DataBus;


class HwDevice
{
public:
    DataBus *mBus = nullptr;
    virtual void tick( uint64_t dt ) {  };

    ubyte busRead( uint16_t );
    void  busWrite( uint16_t, ubyte );
};



class ioDevice: public HwDevice
{
protected:
    ubyte   *mData;
    uint16_t mSize;

public:
    ioDevice( ubyte *data, uint16_t sz )
    :   mData(data), mSize(sz) {  };

    ubyte *ioData();
    virtual ubyte ioRead( uint16_t );
    virtual void ioWrite( uint16_t, ubyte );
};