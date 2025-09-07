#pragma once
#include "addrspace.hpp"

namespace Emu
{
    class HwMapper;
}


class Emu::HwMapper
{
public:
    virtual void write(uword, ubyte) = 0;
    virtual void tick() { };
    virtual bool irqAsserted() const { return false; }

};
