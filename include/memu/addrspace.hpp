#pragma once

#include <memu/types.hpp>
#include <set>


namespace memu
{
    struct Page;
    class AddrSpace;
    class HwModule;
}


struct memu::Page
{
    uint8_t *page; // page base pointer.
    uint16_t base; // base address of mapping. Eg 0x2000-0x2FFF -> base=0x2000.
    uint16_t mask; // wrap-around mask, generally size-1 unless mirroring. Eg 8-1 == addr%8
    Page(void *p, uword b, uword m): page((ubyte*)p), base(b), mask(m) {  };
    Page(): Page(nullptr, 0, 0) {  };
    uint8_t &operator[](int i) { return page[i]; }
};


class memu::AddrSpace
{
private:
    static constexpr size_t PAGE_SIZE = 0x0100;
    std::set<HwModule*> mHwModules;

public:
    memu::Page mRdPages[0x0100];
    memu::Page mWtPages[0x0100];

    AddrSpace();
    void tick();
    void attach(HwModule*);

    ubyte read( uint16_t );
    void write( uint16_t, uint8_t );

    void mapRdPage( uint16_t addr, uint16_t mask, void *page );
    void mapWtPage( uint16_t addr, uint16_t mask, void *page );
    void mapRdWtPage( uint16_t, uint16_t, void* );

    void mapRdRange( uint16_t start, uint16_t end, uint16_t mask, void *pages );
    void mapWtRange( uint16_t start, uint16_t end, uint16_t mask, void *pages );
    void mapRdWtRange( uint16_t, uint16_t, uint16_t, void* );

    void unmapRdPage( uint8_t pageno );
    void unmapWtPage( uint8_t pageno );

};
