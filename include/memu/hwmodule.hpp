#pragma once

#include "types.hpp"

namespace memu
{
    class AddrSpace;
    class HwModule;
}


class memu::HwModule
{
private:
    friend class AddrSpace;

protected:
    size_t mClock;
    void *mCallbackArg;
    void (*on_rdbus)(void*);
    void (*on_wtbus)(void*);

public:
    AddrSpace &mBus;

    HwModule(AddrSpace&);
    HwModule(const HwModule&) = delete;

    ubyte rdbus(addr_t);
    uword rdbusw(addr_t);
    void  wtbus(addr_t, ubyte);


    HwModule &operator=(const HwModule&) = delete;
    size_t clockTime() { return mClock; }

    virtual void tick() = 0;
    virtual void reset() = 0;
};

