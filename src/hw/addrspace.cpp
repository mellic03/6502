#include "addrspace.hpp"
#include "hwmodule.hpp"
#include <memory>

using EADS = Emu::AddrSpace;


EADS::AddrSpace()
{
    static auto dummy = std::unique_ptr<ubyte[]>(new ubyte[256]);
    mapRdRange(0x0000, 0xFFFF, 0xFFFF, dummy.get());
    mapWtRange(0x0000, 0xFFFF, 0xFFFF, dummy.get());

    sizeof(Emu::Page);
}


void EADS::tick()
{
    for (HwModule *hw: mHwModules)
    {
        hw->tick();
    }
}


void EADS::attach( Emu::HwModule *hw )
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



void EADS::mapRdPage( uint16_t addr, uint16_t mask, void *page )
{
    mRdPages[addr>>8] = Emu::Page(page, addr, mask);
}

void EADS::mapWtPage( uint16_t addr, uint16_t mask, void *page )
{
    mWtPages[addr>>8] = Emu::Page(page, addr, mask);
}


void EADS::mapRdRange( uint16_t start, uint16_t end, uint16_t mask, void *pagebase )
{
    ubyte *page = (ubyte*)pagebase;

    for (size_t addr=start; addr<=end; addr+=0x0100)
    {
        mapRdPage(addr, mask, page);
        page += PAGE_SIZE;
    }
}

void EADS::mapWtRange( uint16_t start, uint16_t end, uint16_t mask, void *pagebase )
{
    ubyte *page = (ubyte*)pagebase;

    for (size_t addr=start; addr<=end; addr+=0x0100)
    {
        mapWtPage(addr, mask, page);
        page += PAGE_SIZE;
    }
}




void EADS::unmapRdPage(ubyte pageno ) { mRdPages[pageno] = Emu::Page(); }
void EADS::unmapWtPage(ubyte pageno ) { mWtPages[pageno] = Emu::Page(); }

