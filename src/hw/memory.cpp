#include "memory.hpp"
#include <cassert>
#include <cstring>


uint8_t MemoryRW::rd( uint16_t i )
{
    assert(0<=i && i<mSize);
    return mFront[i];
}

void MemoryRW::wt( uint16_t i, uint8_t x )
{
    assert(0<=i && i<mSize);
    // mBack[i] = x;
    mFront[i] = x;
}

void MemoryRW::Tick()
{
    // memcpy(mFront, mBack, mSize);
}


