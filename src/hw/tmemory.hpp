#pragma once

#include "bus.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>


/**
 * Read/write memory.
 */
template <size_t mSize>
class TMemoryRW: public iBusDevice
{
private:
    uint8_t mBack[mSize];
    uint8_t mFront[mSize];

public:
    virtual uint8_t rd( uint16_t i )
    {
        if (!(i < mSize))
            fprintf(stderr, "%u > %lu\n", i, mSize);
        assert(i < mSize);
        return mFront[i];
    }

    virtual void wt( uint16_t i, uint8_t x )
    {
        if (!(i < mSize))
            fprintf(stderr, "%u > %lu\n", i, mSize);
        assert(i < mSize);
        mBack[i] = x;
    }

    virtual void Tick() override
    {
        memcpy(mFront, mBack, mSize);
    }
};
using MemRW1K  = TMemoryRW<1*1024>;
using MemRW2K  = TMemoryRW<2*1024>;
using MemRW4K  = TMemoryRW<4*1024>;
using MemRW8K  = TMemoryRW<8*1024>;
using MemRW16K = TMemoryRW<16*1024>;
using MemRW32K = TMemoryRW<32*1024>;
using MemRW64K = TMemoryRW<64*1024>;




/**
 * Read-only memory.
 */
template <size_t mSize>
class TMemoryRO: public TMemoryRW<mSize>
{
public:
    virtual void Tick() final {  };
};


/**
 * Write-only memory.
 */
template <size_t mSize>
class TMemoryWO: public TMemoryRW<mSize>
{
public:
    virtual uint8_t rd( uint16_t ) final { return 0; };
};
