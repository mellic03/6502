#pragma once

#include <memu/addrpage.hpp>
#include "rwx.hpp"
#include <set>


namespace memu
{
    class AddrSpace;
    class HwModule;
}


class memu::AddrSpace
{
private:
    iPageEntry *mPages[256];
    std::set<HwModule*> mHwModules;

    void _mapPage(addr_t, addr_t, RWX_, void*);
    void _unmapPage(addr_t, RWX_);

public:
    using RdFunc = uint8_t (*)(HwModule*, addr_t);
    using WtFunc = void (*)(HwModule*, addr_t, uint8_t);

    AddrSpace();
    void tick();
    void attach(HwModule*);
    
    ubyte read(addr_t);
    void write(addr_t, ubyte);
    ubyte operator[](int i) { return read(i); }

    void mapPage(addr_t base, uint16_t mask, RWX_, void*);
    void mapRange(addr_t base, addr_t end, RWX_, void *buf, size_t bufsz);
    void unmapPage(addr_t addr);

};
