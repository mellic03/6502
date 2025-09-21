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
    
    virtual ubyte read(addr_t);
    virtual void  write(addr_t, ubyte);
    ubyte operator[](int i) { return read(i); }

    void mapPage(addr_t base, uint16_t mask, RWX_, void*);
    void mapRange(addr_t base, addr_t end, RWX_, void *buf, size_t bufsz);
    void unmapPage(addr_t addr);


    template <typename iPageType, typename... Args>
    void mapiPage( addr_t addr, uint16_t mask, RWX_ rwx, void *buf,
                   Args... args )
    {
        static const char rwxtab[4][4] = { "..", "R.", ".W", "RW" };
        log::Info("mapPage  %04X  %s\n", addr, (rwx<4 ? rwxtab[rwx] : "??"));
        mPages[addr>>8] = new iPageType(buf, rwx, args...);
    }

    // template <typename iPageType, typename... Args>
    // void mapiRange( addr_t base, addr_t end, RWX_ rwx, void *buf, size_t bufsz,
    //                 Args... args )
    // {
    //     size_t len = end-base + 1;

    //     for (uint16_t i=0; i<len; i+=256)
    //     {
    //         ubyte *pgbase = (ubyte*)buf + (i % bufsz);
    //         mPages[(base+i)>>8] = new iPageType(pgbase, rwx, args...);
    //     }
    // }

    template <typename iPageType, typename... Args>
    void mapiRange( addr_t base, addr_t end, Args&&... args )
    {
        size_t len = end-base + 1;

        for (uint16_t i=0; i<len; i+=256)
        {
            mPages[(base+i)>>8] = new iPageType(args...);
        }
    }

};
