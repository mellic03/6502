#pragma once

#include "bus.hpp"
#include <cassert>



/**
 * Read/write memory.
 */
class ioDevice: public HwDevice
{
public:
    virtual uint8_t rd( uint16_t addr ) = 0;
    virtual void wt( uint16_t addr, uint8_t x ) = 0;
    virtual void Tick() = 0;
};


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
    virtual void Tick() override;
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void Tick() final {  };
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual uint8_t rd( uint16_t ) final { return 0; };
    virtual void Tick() final {  };
};
