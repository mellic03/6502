#pragma once
#include "types.hpp"

namespace memu
{
    class DataLatch;

    template <typename T>
    class DeltaLatch;

    // template <typename T>
    // class ShiftRegister;
}



class memu::DataLatch
{
private:
    bool nextIsHi = true;
    uword &value;

public:
    DataLatch( uword &v ): value(v) { }

    inline void reset() { nextIsHi=true; }
    inline uword read() { return value; }
    inline void write( ubyte data )
    {
        if (nextIsHi) value = (value & 0x00FF) | (uint16_t(data) << 8);
        else          value = (value & 0xFF00) | data;
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




// template <typename T>
// class memu::ShiftRegister
// {
// private:
//     T mValue;

// public:
//     ShiftRegister(): mValue() {  }
//     ShiftRegister(T v): mValue(v) {  }

//     void load( T value )
//     {
//         mValue = value;
//     }

//     bool read()
//     {
//         bool bit = bool(mValue & 0b0001);
//         mValue >>= 1;
//         return bit;
//     }

// };
