#pragma once

#include "bus.hpp"

class DataBus;


class HwDevice
{
public:
    DataBus *mBus;
    HwDevice( DataBus *bus=nullptr ): mBus(bus) {  };

    virtual void tick( uint64_t dt ) {  };

    ubyte busRead( uint16_t );
    void  busWrite( uint16_t, ubyte );
};



class ioDevice
{
protected:
    union {
        void  *mData;
        ubyte *mBytes;
    };
    size_t mSize;

public:
    template <typename T>
    ioDevice(T *p): mData(p), mSize(sizeof(T)) {  };
    ioDevice(void *p, uint16_t s): mData(p), mSize(s) {  };
    ioDevice(): ioDevice(nullptr, 0) {  };

    template <typename T>
    T *data() { return static_cast<T*>(mData); }

    void  *data() { return mData; };
    size_t size() { return mSize; };
    ubyte &operator[](uint16_t i);

    virtual ubyte ioRead( uint16_t );
    virtual void ioWrite( uint16_t, ubyte );
};