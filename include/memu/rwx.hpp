#ifndef MEMU_TYPES_HPP
#define MEMU_TYPES_HPP

#include "types.hpp"

namespace memu
{
    /**
     * Page access flags
     */
    enum RWX_
    {
        RWX_N    = 0 << 0, // none  
        RWX_R    = 1 << 0, // read  
        RWX_W    = 1 << 1, // write 
        RWX_X    = 1 << 2, // exec  
        RWX_RW   = RWX_R | RWX_W | 00000,
        RWX_RX   = RWX_R | 00000 | RWX_X,
        RWX_RWX  = RWX_R | RWX_W | RWX_X,
        RWX_WX   = 00000 | RWX_W | RWX_X
    };
}

#endif
