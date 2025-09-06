#include "memory.hpp"
#include <cassert>
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

void MemoryRO::ioWrite( uint16_t addr, ubyte value )
{
    assert(false);
}

ubyte MemoryWO::ioRead( uint16_t addr )
{
    assert(false);
}


