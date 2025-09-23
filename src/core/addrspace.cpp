#include <memu/addrspace.hpp>
#include <memu/hwmodule.hpp>
#include <memu/log.hpp>
#include <memory>
#include <cstring>

using EADS = memu::AddrSpace;


EADS::AddrSpace()
{
    for (size_t i=0; i<256; i++)
    {
        mRdPages[i] = PageEntry(PageEntry::pgdummy, 0xFF);
        mWtPages[i] = PageEntry(PageEntry::pgdummy, 0xFF);
    }
}



void EADS::tick()
{
    for (HwModule *hw: mHwModules)
    {
        hw->tick();
    }
}


void EADS::reset()
{
    for (HwModule *hw: mHwModules)
        hw->reset();
    memcpy(mSigPrev, mSigCurr, sizeof(mSigCurr));
}


void EADS::attach( memu::HwModule *hw )
{
    mHwModules.insert(hw);
}


ubyte EADS::read( uint16_t i )
{
    auto &pg = mRdPages[i>>8];
    bool  H = (pg.mHandler != nullptr);

    if (H) return pg.mHandler->read(i);
    else   return pg.mBuf[i & pg.mMask];
}


void EADS::write( uint16_t i, ubyte data )
{
    auto &pg = mWtPages[i>>8];
    bool  H = (pg.mHandler != nullptr);

    if (H) pg.mHandler->write(i, data);
    else   pg.mBuf[i & pg.mMask] = data;
}


#include <memu/log.hpp>

void EADS::_mapPage( addr_t addr, uint16_t mask, RWX_ rwx, void *buf )
{
    LogAsrt(addr%256 == 0, "Addr (%04X) must be multiple of 256.", addr);

    // LogAsrt(
    //     mPages[addr>>8]==nullptr,
    //     "Page %04X already mapped.", addr>>8
    // );

    if (rwx & RWX_R) mRdPages[addr>>8] = PageEntry((ubyte*)buf, mask);
    if (rwx & RWX_W) mWtPages[addr>>8] = PageEntry((ubyte*)buf, mask);
}


void EADS::mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *buf )
{
    static const char rwxtab[4][4] = { "..", "R.", ".W", "RW" };
    log::Info("mapPage  %04X  %s\n", addr, (rwx<4 ? rwxtab[rwx] : "??"));
    _mapPage(addr, mask, rwx, buf);
}


void EADS::mapRdRange( addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask )
{
    LogAsrt(base%256 == 0, "Base (%04X) must be multiple of 256.", base);
    LogAsrt((end+1)%256 == 0, "End (%04X) + 1 must be multiple of 256.", end);

    size_t len = (end+1) - base;

    for (size_t off=0; off<len; off+=256)
    {
        uword  addr = base + off;
        uword  boff = ((off % bufsz) >> 8) << 8;
        ubyte *page = (ubyte*)buf + boff;
        mRdPages[addr>>8] = PageEntry(page, mask);
    }
}


void EADS::mapWtRange( addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask )
{
    LogAsrt(base%256 == 0, "Base (%04X) must be multiple of 256.", base);
    LogAsrt((end+1)%256 == 0, "End (%04X) + 1 must be multiple of 256.", end);

    size_t len = (end+1) - base;

    for (size_t off=0; off<len; off+=256)
    {
        uword  addr = base + off;
        uword  boff = ((off % bufsz) >> 8) << 8;
        ubyte *page = (ubyte*)buf + boff;
        mWtPages[addr>>8] = PageEntry(page, mask);
    }
}


void EADS::mapRWRange( addr_t base, addr_t end, void *buf, size_t bufsz, ubyte mask )
{
    mapRdRange(base, end, buf, bufsz, mask);
    mapWtRange(base, end, buf, bufsz, mask);
}



void EADS::mapRange( addr_t base, addr_t end, iPageHandler *handler )
{
    LogAsrt(base%256 == 0, "Base (%04X) must be multiple of 256.", base);
    LogAsrt((end+1)%256 == 0, "End (%04X) + 1 must be multiple of 256.", end);

    for (uint16_t addr=base; addr<end+1; addr+=256)
    {
        mRdPages[addr>>8] = PageEntry(handler);
        mWtPages[addr>>8] = PageEntry(handler);
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

