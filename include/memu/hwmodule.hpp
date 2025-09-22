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
    size_t mClock;

protected:
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

    /**
     * Execute.
     * @return Number of clock cycles used.
     */
    virtual size_t tick() = 0;

    /**
     * Reset the device.
     */
    virtual void reset() = 0;
};

