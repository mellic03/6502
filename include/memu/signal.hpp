#pragma once
#include "types.hpp"

namespace memu
{
    template <typename PinEnum>
    struct SigPins;
}

template <typename PinEnum>
struct memu::SigPins
{
private:
    ubyte mCurr[(size_t)PinEnum::NumValues];
    ubyte mPrev[(size_t)PinEnum::NumValues];

public:
    SigPins()
    {
        for (size_t i=0; i<(size_t)PinEnum::NumValues; i++)
        {
            mCurr[i] = 0;
            mPrev[i] = 0;
        }
    }

    void sigLow (PinEnum p) { mCurr[(ubyte)p] = 0; }
    void sigHigh(PinEnum p) { mCurr[(ubyte)p] = 255; }
    void sigFlip(PinEnum p) { mCurr[(ubyte)p] = ~mCurr[(ubyte)p]; }

};
