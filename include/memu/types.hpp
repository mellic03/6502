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



template <size_t N, typename T>
struct vec;


template <typename T>
struct vec<1, T>
{
    union { T x, r; };
};


template <typename T>
struct vec<2, T>: public vec<1, T>
{
    union { T y, g; };
};


template <typename T>
struct vec<3, T>: public vec<2, T>
{
    union { T z, b; };
};


template <typename T>
struct vec<4, T>: public vec<3, T>
{
    union { T w, a; };

    // void sasad()
    // {
    //     vec<3, T>::
    // }

    // T &operator[](int i)
    // {
    //     switch (i)
    //     {
    //         default: return x;
    //         case 0: return x;
    //         case 1: return y;
    //         case 2: return vec<3, T>::z;
    //         case 3: return w;
    //     }
    // }
};


using ivec2 = vec<2, int>;
using ivec3 = vec<2, int>;
using ivec4 = vec<3, int>;

using uvec2 = vec<2, uint32_t>;
using uvec3 = vec<3, uint32_t>;
using uvec4 = vec<4, uint32_t>;

