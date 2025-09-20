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



struct ivec1        { union { int x, r; }; };
struct ivec2: ivec1 { union { int y, g; }; };
struct ivec3: ivec2 { union { int z, b; }; };
struct ivec4: ivec3 { union { int w, a; }; };

