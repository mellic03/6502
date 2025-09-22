#pragma once

#include "types.hpp"
#include "addrpage.hpp"
#include "log.hpp"
#include "signal.hpp"
#include "rwx.hpp"
#include <set>


namespace memu
{
    enum class SIG: uint8_t
    {
        IRQ, RES, NMI, WAI,
        NumValues
    };

    class AddrSpace;
    class HwModule;
}


class memu::AddrSpace: public SigPins<SIG>
{
private:
    PageEntry mRdPages[256];
    PageEntry mWtPages[256];
    std::set<HwModule*> mHwModules;

    void _mapPage(addr_t, addr_t, RWX_, void*);
    void _unmapPage(addr_t, RWX_);

public:
    using RdFunc = uint8_t (*)(HwModule*, addr_t);
    using WtFunc = void (*)(HwModule*, addr_t, uint8_t);

    uint8_t prev_nmi = 0;
    uint8_t prev_irq = 1;
    uint8_t line_nmi = 0; // Active on rising edge.
    uint8_t line_irq = 1; // Active on low.
    uint8_t pend_nmi = 0;
    uint8_t pend_irq = 0;

    AddrSpace();
    void tick();
    void reset();
    void attach(HwModule*);
    
    virtual ubyte read(addr_t);
    virtual void  write(addr_t, ubyte);
    ubyte operator[](int i) { return read(i); }

    void mapPage(addr_t base, uint16_t mask, RWX_, void*);
    void mapRdRange(addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask=0xFF);
    void mapWtRange(addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask=0xFF);
    void mapRWRange(addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask=0xFF);

    void mapRange(addr_t base, addr_t end, iPageHandler*);
    void unmapPage(addr_t addr);

};
