#include <memu/addrpage.hpp>
#include <memu/log.hpp>

using namespace memu;



// iPageEntry::iPageEntry(addr_t addr, RWX_ rwx)
// :   mBase(addr),
//     mRWX(rwx)
// {
//     LogAsrt((addr%256)==0, "addr (%04X) must be multiple of 0x0100.\n", addr);
// }



// ubyte iPageEntry::read(addr_t addr)
// {
//     uword pno = (addr >> 8);
//     uword idx = (addr & 0x00FF);

//     //  Base
//     // [0000] [0000] [0000] [0000]
//     // 
//     //
//     //
//     //

//     // LogAsrt(i<256, "Index %u out of bounds.\n", i);

//     // if (!(mRWX & RWX_R))
//     // {
//     //     LogAsrt(mRWX & RWX_R, "Cannot read %04X (page %04X not read-mapped)\n", mAddr+i, mAddr);
//     //     return 0;
//     // }

//     return this->_read(addr);
// }


// void iPageEntry::write(addr_t addr, ubyte v)
// {
//     // LogAsrt(i<256, "Index %u out of bounds.\n", i);

//     // if (!(mRWX & RWX_W))
//     // {
//     //     LogAsrt(mRWX & RWX_W, "Cannot write %04X (page %04X not write-mapped)\n", mAddr+i, mAddr);
//     //     return;
//     // }

//     this->_write(addr, v);
// }



ubyte PageEntry::read( addr_t addr )
{
    uint16_t idx = addr & 0x00FF;
    return mBuf[idx];
}


void PageEntry::write( addr_t addr, ubyte data )
{
    uint16_t idx = addr & 0x00FF;
    mBuf[idx] = data;
}


