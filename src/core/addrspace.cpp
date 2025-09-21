#include <memu/addrspace.hpp>
#include <memu/hwmodule.hpp>
#include <memu/log.hpp>
#include <memory>

using EADS = memu::AddrSpace;


EADS::AddrSpace()
{
    for (size_t i=0; i<256; i++)
    {
        mRdPages[i] = PageEntry();
        mWtPages[i] = PageEntry();
    }
}



void EADS::tick()
{
    for (HwModule *hw: mHwModules)
        hw->mClock += hw->tick();
}


void EADS::reset()
{
    for (HwModule *hw: mHwModules)
        hw->reset();
}


void EADS::attach( memu::HwModule *hw )
{
    mHwModules.insert(hw);
}


ubyte EADS::read( uint16_t addr )
{
    return mRdPages[addr>>8].read(addr);
    // LogAsrt(pg.used, "Cannot read %04X (page %04X not read-mapped)\n", i, (i>>8)<<8);
    // uint8_t idx = (i & pg.mask);
    // if (pg.rdfn)  return pg.rdfn(pg.dev, idx);
    // else          return pg.page[idx];
}


void EADS::write( uint16_t addr, ubyte data )
{
    mWtPages[addr>>8].write(addr, data);
    // LogAsrt(pg.used, "Cannot write %04X (page %04X not write-mapped)\n", i, (i>>8)<<8);
    // uint8_t idx = (i & pg.mask);
    // if (pg.wtfn)  pg.wtfn(pg.dev, idx, v);
    // else          pg.page[idx] = v;
}


#include <memu/log.hpp>

void EADS::_mapPage( addr_t addr, uint16_t mask, RWX_ rwx, void *buf )
{
    LogAsrt(addr%256 == 0, "Addr (%04X) must be multiple of 256.", addr);

    // LogAsrt(
    //     mPages[addr>>8]==nullptr,
    //     "Page %04X already mapped.", addr>>8
    // );

    if (rwx & RWX_R) mRdPages[addr>>8] = PageEntry((ubyte*)buf);
    if (rwx & RWX_W) mWtPages[addr>>8] = PageEntry((ubyte*)buf);
}


void EADS::mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *buf )
{
    static const char rwxtab[4][4] = { "..", "R.", ".W", "RW" };
    log::Info("mapPage  %04X  %s\n", addr, (rwx<4 ? rwxtab[rwx] : "??"));
    _mapPage(addr, mask, rwx, buf);
}


void EADS::mapRange( addr_t base, addr_t end, RWX_ rwx, void *buf, size_t bufsz )
{
    LogAsrt(base%256 == 0, "Base (%04X) must be multiple of 256.", base);
    LogAsrt((end+1)%256 == 0, "End (%04X) + 1 must be multiple of 256.", end);

    size_t len = (end+1) - base;

    for (size_t off=0; off<len; off+=256)
    {
        uword  addr = base + off;
        size_t boff = (off % bufsz) & ~0xFF;
        ubyte *page = (ubyte*)buf + boff;
        _mapPage(addr, 0xFFFF, rwx, page);
    }
}


void EADS::mapRange( addr_t base, addr_t end, iPageHandler *handler )
{
    LogAsrt(base%256 == 0, "Base (%04X) must be multiple of 256.", base);
    LogAsrt((end+1)%256 == 0, "End (%04X) + 1 must be multiple of 256.", end);

    size_t len = end-base + 1;

    for (uint16_t addr=base; addr<end+1; addr+=256)
    {
        mRdPages[addr>>8] = PageEntry();
        mRdPages[addr>>8].mHandler = handler;
        mWtPages[addr>>8] = mRdPages[addr>>8];
    }  
}




void EADS::_unmapPage( addr_t addr, RWX_ rwx )
{
    // LogAsrt(
    //     mPages[addr>>8]!=nullptr,
    //     "Page %04X not mapped.", addr>>8
    // );

    // delete mPages[addr>>8];
}


void EADS::unmapPage( addr_t addr )
{
    log::Warn("unmapPage %04X\n", addr);
    _unmapPage(addr, RWX_RW);
}

