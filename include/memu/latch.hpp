#pragma once
#include "types.hpp"

namespace memu
{
    class DataLatch;

    template <typename T>
    class DeltaLatch;
}


class memu::DataLatch
{
private:
    bool nextIsHi = true;

    union {
        uword word;
        struct {
            ubyte lo;
            ubyte hi;
        } __attribute__((packed));
    };

public:
    DataLatch( uword w ): word(w) { }

    inline uword read()
    {
        return word;
    }

    inline void write( ubyte data )
    {
        if (nextIsHi)
            word = (word & 0x00FF) | (uint16_t(data) << 8);
        else
            word = (word & 0xFF00) | data;

        nextIsHi = !nextIsHi;
    }
};



template <typename T>
class memu::DeltaLatch
{
private:
    T   mBuf[2];
    int mIdx;

public:
    DeltaLatch(): mIdx(0) { }

    T read()
    {
        T *data = mBuf[mIdx];
        mIdx = (mIdx+1) % 2;
        return *data;
    }

    void write( T data )
    {
        mBuf[mIdx] = data;
        mIdx = (mIdx+1) % 2;
    }
};

