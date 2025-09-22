#pragma once
#include "types.hpp"

namespace memu
{
    class SigLatch;

    template <typename PinEnum>
    class SigPins;
}


class memu::SigLatch
{
private:
    bool curr = false;   // current line state
    bool prev = false;   // previous line state
    bool latched = false; // whether the event has been latched

public:
    SigLatch() = default;

    void set( bool level )
    { 
        curr = level; 
        if (!prev && curr)
            latched = true;
        prev = curr;
    }

    void clear() { latched = false; }
    bool triggered() const { return latched; }
    bool active() const { return curr; }
    bool risingEdge() const { return !prev && curr; }
    bool fallingEdge() const { return prev && !curr; }
};


template <typename PinEnum>
class memu::SigPins
{
protected:
    ubyte mSigCurr[(size_t)PinEnum::NumValues];
    ubyte mSigPrev[(size_t)PinEnum::NumValues];

public:
    SigPins()
    {
        for (size_t i=0; i<(size_t)PinEnum::NumValues; i++)
        {
            mSigCurr[i] = 0;
            mSigPrev[i] = 0;
        }
    }

    bool sigLo(PinEnum p) { return (mSigCurr[(ubyte)p] == 0); }
    bool sigHi(PinEnum p) { return (mSigCurr[(ubyte)p] == 1); }
    bool sigEd(PinEnum p) { return (mSigCurr[(ubyte)p] && !mSigPrev[(ubyte)p]); }

    void sigSet(PinEnum p, uint8_t v)
    {
        // mPrev[(ubyte)p] = mCurr[(ubyte)p];
        mSigCurr[(ubyte)p] = (v) ? 1 : 0;
    }

    void sigFlip(PinEnum p)
    {
        mSigCurr[(ubyte)p] = (mSigCurr[(ubyte)p]) ? 1 : 0;
    }

};
