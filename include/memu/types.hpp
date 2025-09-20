#pragma once

#include <stddef.h>
#include <stdint.h>

using si08 = int8_t;
using si16 = int16_t;
using si32 = int32_t;
using si64 = int64_t;

using ui08 = uint8_t;
using ui16 = uint16_t;
using ui32 = uint32_t;
using ui64 = uint64_t;

using ubyte = uint8_t;
using uword = uint16_t;


#define MEMU_ADDRSPACE_16BIT

#ifdef MEMU_ADDRSPACE_8BIT
    using addr_t = uint8_t;
#elif defined(MEMU_ADDRSPACE_16BIT)
    using addr_t = uint16_t;
#elif defined(MEMU_ADDRSPACE_32BIT)
    using addr_t = uint32_t;
#elif defined(MEMU_ADDRSPACE_64BIT)
    using addr_t = uint32_t;
#endif



struct ivec1
{
    union { int x, r; };
    // int &operator[](int) { return x; }
};


struct ivec2: ivec1
{
    union { int y, g; };
    // int &operator[](int i)
    // {
    //     switch (i)
    //     {
    //         default: return ivec1::operator[](i);
    //         case 1: return y;
    //     }
    // }
};


struct ivec3: ivec2
{
    union { int z, b; };
    // int &operator[](int i)
    // {
    //     switch (i)
    //     {
    //         default: return ivec2::operator[](i);
    //         case 2: return z;
    //     }
    // }
};


struct ivec4: ivec3
{
    union { int w, a; };

    int &operator[](int i)
    {
        switch (i)
        {
            default: return x;
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }
};

