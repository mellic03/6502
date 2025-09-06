#pragma once

// #include "bus.hpp"
#include "device.hpp"
#include <cassert>


/**
 * Read/write memory.
 */
class MemoryRW: public ioDevice
{
public:
    MemoryRW(): ioDevice(nullptr, 0) {  };
    MemoryRW( ubyte *d, size_t s ): ioDevice(d, s) {  };

    // void reset( size_t sz )
    // {
    //     if (mBack)  delete[] mBack;
    //     if (mFront) delete[] mFront;

    //     mSize  = sz;
    //     mBack  = new uint8_t[sz];
    //     mFront = new uint8_t[sz];
    // }

    // virtual void flash( uint8_t *src, size_t sz );
    // virtual void flush();
    // virtual void tick( uint64_t dt ) override;
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void ioWrite(uint16_t, ubyte) final;
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual ubyte ioRead(uint16_t) final;
};
