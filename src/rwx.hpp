#pragma once
#include <cstdint>

struct RWX
{
    enum {
        Z   = 0b000,
        R   = 0b001,
        W   = 0b010,
        RW  = 0b011,
        x2  = 0b100,
        Rx2 = R | x2,
        Wx2 = W | x2,
    };
};
