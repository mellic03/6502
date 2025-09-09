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
    AddrSpace &mBus;

protected:
    uint8_t rdbus(uint16_t); // { return mBus.read(a); }
    void    wtbus(uint16_t, uint8_t); // { mBus.write(a, v); }

public:
    HwModule(AddrSpace&);
    virtual void tick() = 0;
    virtual void reset() = 0;
};

