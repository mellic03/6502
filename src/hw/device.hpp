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



class ioDevice
{
protected:
    ubyte   *mData;
    uint16_t mSize;

public:
    ioDevice(void *p, uint16_t s): mData((uint8_t*)p), mSize(s) {  };
    ioDevice(): ioDevice(nullptr, 0) {  };
    ubyte *data() { return mData; };
    ubyte &operator[](uint16_t i);

    virtual ubyte ioRead( uint16_t );
    virtual void ioWrite( uint16_t, ubyte );
};