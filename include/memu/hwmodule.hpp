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
    uint8_t rdbus(uint16_t); // { return mBus.read(a); }
    void    wtbus(uint16_t, uint8_t); // { mBus.write(a, v); }

public:
    AddrSpace &mBus;

    HwModule(AddrSpace&);
    HwModule(const HwModule&) = delete;
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

