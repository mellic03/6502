#pragma once

#include <memu/addrpage.hpp>
#include <memu/log.hpp>
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
    PageEntry mRdPages[256];
    PageEntry mWtPages[256];
    std::set<HwModule*> mHwModules;

    void _mapPage(addr_t, addr_t, RWX_, void*);
    void _unmapPage(addr_t, RWX_);

public:
    using RdFunc = uint8_t (*)(HwModule*, addr_t);
    using WtFunc = void (*)(HwModule*, addr_t, uint8_t);

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
