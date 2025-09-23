#pragma once

#include "types.hpp"


namespace memu
{
    inline ubyte ioRead(ubyte *p)
    {
        return (p) ? *p : 0;
    }

    inline void ioWrite(ubyte *p, ubyte v)
    {
        if (p) *p=v;
    }

}