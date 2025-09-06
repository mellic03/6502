#include "memory.hpp"
#include <cassert>
#include <cstring>


uint8_t MemoryRW::rd( uint16_t i )
{
    assert(i < mSize);
    return mFront[i];
}

void MemoryRW::wt( uint16_t i, uint8_t x )
{
    assert(i < mSize);
    mBack[i] = x;
}

void MemoryRW::flash( uint8_t *src, size_t sz )
{
    assert(!(sz > mSize));
    memcpy(mBack, src, sz);
    flush();
}

void MemoryRW::flush()
{
    memcpy(mFront, mBack, mSize);
}

void MemoryRW::Tick()
{
    flush();
}


