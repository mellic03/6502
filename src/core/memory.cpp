#include <memu/memory.hpp>
#include <cassert>
#include <cstdio>
#include <cstring>


// void MemoryRW::flash( uint8_t *src, size_t sz )
// {
//     assert(!(sz > mSize));
//     memcpy(mBack, src, sz);
//     flush();
// }

// void MemoryRW::flush()
// {
//     memcpy(mFront, mBack, mSize);
// }

// void MemoryRW::tick( uint64_t dt )
// {
//     flush();
// }

ubyte MemoryRW::read( addr_t addr )
{
    assert(addr < mSize);
    return mData[addr];
}

void MemoryRW::write( addr_t addr, ubyte value )
{
    assert(addr < mSize);
    mData[addr] = value;
}


void MemoryRO::write( addr_t addr, ubyte value )
{
    fprintf(stderr, "Attempted write to read-only device.\n");
    assert(false);
}

ubyte MemoryWO::read( addr_t addr )
{
    fprintf(stderr, "Attempted read of write-only device.\n");
    assert(false);
}


