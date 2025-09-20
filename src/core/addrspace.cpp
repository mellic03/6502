#include <memu/addrspace.hpp>
#include <memu/hwmodule.hpp>
#include <memu/log.hpp>
#include <memory>

using EADS = memu::AddrSpace;
ubyte *EADS::PgEntry::dummy = new ubyte[256];


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


ubyte EADS::read( uint16_t i )
{
    auto &pg = mRdPages[i>>8];
    LogAsrt(pg.used, "Cannot read %04X (page %04X not read-mapped)\n", i, (i>>8)<<8);

    uint8_t idx = (i & pg.mask) & 0x00FF;
    if (pg.rdfn) return pg.rdfn(pg.dev, idx);
    else         return pg.page[idx];
}


void EADS::write( uint16_t i, ubyte v )
{
    auto &pg = mWtPages[i>>8];
    LogAsrt(pg.used, "Cannot write %04X (page %04X not write-mapped)\n", i, (i>>8)<<8);

    uint8_t idx = (i & pg.mask) & 0x00FF;
    if (pg.wtfn) pg.wtfn(pg.dev, idx, v);
    else         pg.page[idx] = v;
}



#include <memu/log.hpp>
static const char rwxtab[4][4] = { "..", "R.", ".W", "RW" };

void EADS::mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *page )
{
    log::Info("mapPage  %04X       %s\n", addr, (rwx<4 ? rwxtab[rwx] : "??"));
    _mapPage(addr, mask, rwx, page);
}


void EADS::mapRange( addr_t base, addr_t end, addr_t mask, RWX_ rwx,
                     void *pages )
{
    log::Info("mapRange %04X-%04X  %s\n", base, end, (rwx<4 ? rwxtab[rwx] : "??"));

    LogAsrt(base % 256==0, "base must be page addr, supplied %04X\n", base);
    LogAsrt((end+1) % 256==0, "end must be page addr-1, supplied %04X\n", end);

    size_t len = (end+1) - base;
    for (size_t off=0; off<len; off+=256)
    {
        _mapPage(base+off, mask, rwx, (ubyte*)pages + off);
    }
}


void EADS::mapRangeTiny( addr_t base, addr_t end, HwModule *dev, RdFunc rd, WtFunc wt )
{
    size_t len = (end+1) - base;

    for (size_t i=base; i<base+len; i+=256)
    {
        auto &rdpg = mRdPages[i>>8];
        auto &wtpg = mWtPages[i>>8];

        rdpg.mask = 0xFFFF;
        rdpg.used = true;
        rdpg.dev  = dev;
        rdpg.rdfn = rd;
        rdpg.wtfn = wt;

        mWtPages[i>>8] = rdpg;
    }
}





void EADS::unmapPage( addr_t addr )
{
    log::Warn("unmapPage %04X\n", addr);
    _unmapPage(addr, RWX_RW);
}


void EADS::unmapRange( addr_t base, addr_t end )
{
    log::Warn("unmapRange %04X-%04X\n", base, end);

    size_t len = (end+1) - base;
    for (size_t off=0; off<len; off+=256)
    {
        _unmapPage(base+off, RWX_RW);
    }
}


void EADS::_mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *page )
{
    LogAsrt((addr%256)==0, "address must be page base, supplied %04X\n", addr);

    if (rwx & RWX_R)
    {
        auto &pg = mRdPages[addr>>8];
        LogAsrt(!pg.used, "Page %04X already read-mapped\n", addr);
        pg = PgEntry(page, mask, true);
    }

    if (rwx & RWX_W)
    {
        auto &pg = mWtPages[addr>>8];
        LogAsrt(!pg.used, "Page %04X already write-mapped\n", addr);
        pg = PgEntry(page, mask, true);
    }
}


void EADS::_unmapPage( addr_t addr, RWX_ )
{
    LogAsrt((addr%256)==0, "address must be page base, supplied %04X\n", addr);

    // if (rwx & RWX_R)
    {
        auto &pg = mRdPages[addr>>8];
        LogAsrt(pg.used, "Page %04X not read-mapped", addr);
        pg = PgEntry();
    }

    // if (rwx & RWX_W)
    {
        auto &pg = mWtPages[addr>>8];
        LogAsrt(pg.used, "Page %04X not write-mapped", addr);
        pg = PgEntry();
    }
}

