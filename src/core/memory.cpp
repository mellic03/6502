#include <memu/memory.hpp>
#include <memu/log.hpp>
#include <cstdio>
#include <cstring>

using namespace memu;


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

void MemoryRW::_check( addr_t i )
{
    LogAsrt(i<mSize, "Out of bounds access: mData[%u], mSize=%lu\n", i, mSize);
}

ubyte *MemoryRW::get( addr_t i )
{
    _check(i);
    return &mData[i];
}

void MemoryRO::write( addr_t i, ubyte value )
{
    log::Warn("Attempted write to read-only memory.\n");
}

ubyte MemoryWO::read( addr_t i )
{
    log::Warn("Attempted read of write-only memory.\n");
    return 0;
}


