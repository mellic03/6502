#pragma once

// #include "bus.hpp"
#include "device.hpp"
#include <cassert>


/**
 * Read/write memory.
 */
class MemoryRW: public ioDevice
{
private:
    size_t   mSize;
    uint8_t *mBack;
    uint8_t *mFront;

public:
    void reset( size_t sz )
    {
        if (mBack)  delete[] mBack;
        if (mFront) delete[] mFront;

        mSize  = sz;
        mBack  = new uint8_t[sz];
        mFront = new uint8_t[sz];
    }

    uint8_t *data() { return mBack; }
    size_t   size() { return mSize; }

    virtual uint8_t rd( uint16_t ) override;
    virtual void wt( uint16_t, uint8_t ) override;
    virtual void flash( uint8_t *src, size_t sz );
    virtual void flush();
    virtual void tick( uint64_t dt ) override;
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void tick( uint64_t dt ) final {  };
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual uint8_t rd( uint16_t ) final { return 0; };
    virtual void tick( uint64_t dt ) final {  };
};
