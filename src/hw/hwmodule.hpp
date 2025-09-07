#pragma once

#include "addrspace.hpp"


namespace Emu
{
    class HwModule;
}


class Emu::HwModule
{
private:
    AddrSpace &mBus;

protected:
    uint8_t rdbus(uint16_t a) { return mBus.read(a); }
    void    wtbus(uint16_t a, uint8_t v) { mBus.write(a, v); }

public:
    HwModule(AddrSpace&);
    virtual void tick() = 0;
    virtual void reset() = 0;
};

