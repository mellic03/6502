#pragma once

#include "types.hpp"
#include "rwx.hpp"
#include <set>


namespace memu
{
    class AddrSpace;
    class HwModule;
}


class memu::AddrSpace
{
public:
    using RdFunc = uint8_t (*)(HwModule*, addr_t);
    using WtFunc = void (*)(HwModule*, addr_t, uint8_t);

    AddrSpace() {  };
    void tick();
    void attach(HwModule*);
    
    ubyte read(addr_t);
    void write(addr_t, ubyte);
    ubyte operator[](int i) { return read(i); }

    void mapPage(addr_t base, addr_t mask, RWX_, void*);
    void mapRange(addr_t base, addr_t end, addr_t mask, RWX_, void *buf); // , uword bufsz);
    void mapRangeTiny(addr_t, addr_t, HwModule*, RdFunc, WtFunc);

    void mapRWRange(addr_t a, addr_t b, void *p) { mapRange(a, b, b-a, RWX_RW, p); }
    void mapRdRange(addr_t a, addr_t b, void *p) { mapRange(a, b, b-a, RWX_R, p); }
    void mapWtRange(addr_t a, addr_t b, void *p) { mapRange(a, b, b-a, RWX_W, p); }

    void unmapPage(addr_t addr);
    void unmapRange(addr_t start, addr_t end);

// private:
    class PgEntry
    {
    private:
        static ubyte *dummy;

    public:
        ubyte *page; // page base pointer
        uword  base;
        uword  mask; // wrap-around mask, generally size-1 unless mirrored. Eg 8-1 == addr%8
        bool   used;

        HwModule *dev  = nullptr; // single-byte pages
        RdFunc    rdfn = nullptr; // ...
        WtFunc    wtfn = nullptr; // ...
        
        PgEntry(void *p, uword b, uword m, bool u): page((ubyte*)p), base(b), mask(m), used(u) {  }
        PgEntry(): PgEntry(dummy, 0x00, 0x00, false) {  }
    };

    PgEntry mRdPages[256];
    PgEntry mWtPages[256];
    std::set<HwModule*> mHwModules;

    void _mapPage(addr_t, addr_t, RWX_, void*);
    void _unmapPage(addr_t, RWX_);

};
