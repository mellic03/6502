// #pragma once

// #include "bus.hpp"
// #include <cassert>
// #include <cstring>
// #include <cstdio>


// /**
//  * Read/write memory.
//  */
// template <size_t mSize>
// class TMemoryRW: public HwDevice
// {
// private:
//     uint8_t mBack[mSize];
//     uint8_t mFront[mSize];

// public:
//     uint8_t *data() { return &mBack[0]; }
//     size_t   size() { return mSize; } 

//     virtual uint8_t rd( uint16_t i )
//     {
//         if (!(i < mSize))
//             fprintf(stderr, "%u > %lu\n", i, mSize);
//         assert(i < mSize);
//         return mFront[i];
//     }

//     virtual void wt( uint16_t i, uint8_t x )
//     {
//         if (!(i < mSize))
//             fprintf(stderr, "%u > %lu\n", i, mSize);
//         assert(i < mSize);
//         mBack[i] = x;
//     }

//     virtual void tick( uint64_t dt ) override
//     {
//         memcpy(mFront, mBack, mSize);
//     }
// };
// using Memory1kRW  = TMemoryRW<1*1024>;
// using Memory2kRW  = TMemoryRW<2*1024>;
// using Memory4kRW  = TMemoryRW<4*1024>;




// /**
//  * Read-only memory.
//  */
// template <size_t mSize>
// class TMemoryRO: public TMemoryRW<mSize>
// {
// public:
//     virtual void tick( uint64_t dt ) final {  };
// };


// /**
//  * Write-only memory.
//  */
// template <size_t mSize>
// class TMemoryWO: public TMemoryRW<mSize>
// {
// public:
//     virtual uint8_t rd( uint16_t ) final { return 0; };
// };
