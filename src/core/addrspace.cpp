#include <memu/addrspace.hpp>
#include <memu/hwmodule.hpp>
#include <memory>

using EADS = memu::AddrSpace;


EADS::AddrSpace()
{
    static uint8_t *dummy = new uint8_t[256];
    mapRange(0x0000, 0xFFFF, 0xFFFF, RWX_RW, dummy);
}


void EADS::tick()
{
    for (HwModule *hw: mHwModules)
    {
        hw->tick();
    }
}


void EADS::attach( memu::HwModule *hw )
{
    mHwModules.insert(hw);
}


ubyte EADS::read( uint16_t i )
{
    return mRdPages[i>>8][i&255];
}


void EADS::write( uint16_t i, ubyte v )
{
    mWtPages[i>>8][i&255] = v;
}


void EADS::mapPage( addr_t addr, addr_t mask, RWX_ rwx, void *page )
{
    if (rwx & RWX_R)
        mRdPages[addr>>8] = memu::Page(page, addr, mask);
    if (rwx & RWX_W)
        mWtPages[addr>>8] = memu::Page(page, addr, mask);
}

// #include <stdio.h>
void EADS::mapRange( addr_t start, addr_t end, addr_t mask, RWX_ rwx,
                     void *pages )
{
    // printf("mapRange %04X-%04X %02X %04X &%p\n", start, end, mask, rwx, pages);

    ubyte *page = (ubyte*)pages;

    for (size_t addr=start; addr<=end; addr+=0x0100)
    {
        mapPage(addr, mask, rwx, page);
        page += PAGE_SIZE;
    }
}

