#pragma once

#include <memu/hw/ricoh/2A03.hpp>
#include <memu/addrspace.hpp>


namespace NesEmu
{
    class System;
}


class NesCPU: public memu::Ricoh2A03
{
public:
    using Ricoh2A03::Ricoh2A03;

    class PageHandlerMMIO: public memu::iPageHandler
    {
    private:
        NesEmu::System &nes;
    public:
        PageHandlerMMIO(NesEmu::System &n): nes(n) {  }
        virtual ubyte read(addr_t) final;
        virtual void write(addr_t, ubyte) final;
    };

private:
    friend class PageHandlerMMIO;
    ubyte mmio[0x17 + 1];

};


