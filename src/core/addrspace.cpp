#include <memu/addrspace.hpp>
#include <memu/hwmodule.hpp>
#include <memu/log.hpp>
#include <memory>

using EADS = memu::AddrSpace;


EADS::AddrSpace()
{
    static ubyte *dummy = new ubyte[256];

    for (size_t i=0; i<=256; i++)
    {
        mPages[i] = PageEntry();
    }
}



void EADS::tick()
{
    for (HwModule *hw: mHwModules)
    {
        hw->mClock += hw->tick();
    }
}


void EADS::attach( memu::HwModule *hw )
{
    mHwModules.insert(hw);
}


ubyte EADS::read( uint16_t addr )
{
    return mPages[addr>>8].read(addr);
    // LogAsrt(pg.used, "Cannot read %04X (page %04X not read-mapped)\n", i, (i>>8)<<8);
    // uint8_t idx = (i & pg.mask);
    // if (pg.rdfn)  return pg.rdfn(pg.dev, idx);
    // else          return pg.page[idx];
}


void EADS::write( uint16_t addr, ubyte data )
{
    mPages[addr>>8].write(addr, data);
    // LogAsrt(pg.used, "Cannot write %04X (page %04X not write-mapped)\n", i, (i>>8)<<8);
    // uint8_t idx = (i & pg.mask);
    // if (pg.wtfn)  pg.wtfn(pg.dev, idx, v);
    // else          pg.page[idx] = v;
}


#include <memu/log.hpp>

void EADS::_mapPage( addr_t addr, uint16_t mask, RWX_ rwx, void *buf )
{
    // LogAsrt(
    //     mPages[addr>>8]==nullptr,
    //     "Page %04X already mapped.", addr>>8
    // );

    mPages[addr>>8] = PageEntry((ubyte*)buf, rwx);
}


void EADS::_unmapPage( addr_t addr, RWX_ rwx )
{
    // LogAsrt(
    //     mPages[addr>>8]!=nullptr,
    //     "Page %04X not mapped.", addr>>8
    // );

    // delete mPages[addr>>8];
}



void EADS::mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *buf )
{
    static const char rwxtab[4][4] = { "..", "R.", ".W", "RW" };
    log::Info("mapPage  %04X  %s\n", addr, (rwx<4 ? rwxtab[rwx] : "??"));
    _mapPage(addr, mask, rwx, buf);
}


void EADS::mapRange(addr_t base, addr_t end, RWX_ rwx, void *buf, size_t bufsz)
{
    size_t len = end-base + 1;

    uint16_t off = 0;
    while (off < len)
    {
        ubyte *page = (ubyte*)buf + (off % bufsz);

        mPages[(base+off) >> 8] = PageEntry(page, rwx);
        off += 256;
    }
}


void EADS::mapRange(addr_t base, addr_t end, iPageHandler *handler)
{
    size_t len = end-base + 1;

    for (uint16_t addr=base; addr<end+1; addr+=256)
    {
        mPages[addr>>8] = PageEntry();
        mPages[addr>>8].mHandler = handler;
    }  
}


void EADS::unmapPage( addr_t addr )
{
    log::Warn("unmapPage %04X\n", addr);
    _unmapPage(addr, RWX_RW);
}

