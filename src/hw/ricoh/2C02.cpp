#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
#include <memu/pinout.hpp>

#include <cstdio>
#include <cstring>
#include <ctime>

using namespace memu;



Ricoh2C02::Ricoh2C02( AddrSpace &bus, ubyte *fb, size_t pitch, size_t bpp )
:   HwModule(bus),
    BaseHw(),
    mFramebuffer(fb),
    mPitch(pitch),
    mBPP(bpp),
    mCycle(0),
    mScanLine(0)
{
    static constexpr ubyte blue   [] = { 0x11, 32, 0x36, 203, 79, 33, 128, 219, 80, 179, 4, 36, 25, 35, 183, 220, 46, 160, 80, 142, 25, 43, 6, 107, 214, 14, 247, 240, 149, 61, 225, 176 };
    static constexpr ubyte pink   [] = { 0x11, 246, 108, 92, 187, 42, 210, 95, 168, 78, 142, 98, 129, 239, 103, 87, 172, 13, 30, 101, 140, 87, 215, 181, 232, 143, 132, 245, 39, 10, 61, 155 };
    static constexpr ubyte purple [] = { 0x11, 227, 188, 126, 51, 108, 242, 50, 62, 34, 86, 167, 40, 63, 8, 141, 51, 132, 202, 219, 88, 130, 97, 206, 73, 196, 175, 197, 55, 172, 109, 187 };
    static constexpr ubyte luigi  [] = { 0x11, 89, 54, 254, 96, 12, 40, 67, 229, 115, 37, 146, 57, 101, 190, 224, 85, 64, 203, 98, 106, 182, 27, 196, 254, 231, 87, 157, 17, 71, 189, 134 };
    static constexpr ubyte ginger [] = { 0x11, 16, 22, 214, 245, 224, 70, 255, 64, 116, 122, 27, 99, 72, 85, 139, 182, 112, 82, 127, 92, 88, 47, 23, 185, 17, 132, 75, 47, 215, 16, 147 };

    memcpy(mPaletteCtl, blue, sizeof(mPaletteCtl));
}


int Ricoh2C02::tickn( int nCycles )
{
    if (memu::ioRead(ioRES) == 0)
    {
        this->reset();
    }

    bool visible = (0<=mScanLine && mScanLine<240 && 1<=mCycle && mCycle<=256);
    int leftover = std::max(0, (mCycle+nCycles) - (visible ? 257 : 342));

    if (visible)
    {
        _quik(nCycles-leftover);
    }

    if (mScanLine==241 && mCycle==1)
    {
        ppustat.VBlank = 1;
        // if (ppuctl.NMIEnabled)
        {
            memu::ioWrite(ioINT, 1);
        }
    }


    mCycle += nCycles-leftover;

    if (mCycle >= 341)
    {
        mCycle = 0;
        mScanLine++;

        if (mScanLine >= 261)
        {
            ppustat.VBlank = 0;
            mWin->flush();
            mScanLine = -1;
        }
    }

    return leftover;
}



// void Ricoh2C02::tick()
// {
//     if (memu::ioRead(ioRES) == 0)
//     {
//         this->reset();
//     }

//     if (onRange(0, 240))
//     {
//         uword base = 0x2000 + 0x400*ppuctl.NameTabSel;
//         _drawNameTableRow(ubyte(mScanLine/8));
//     }

//     if (mScanLine==241 && mCycle==1)
//     {
//         ppustat.VBlank = 1;
    
//         // if (ppuctl.NMIEnabled)
//         {
//             memu::ioWrite(ioINT, 1);
//         }
//     }


//     if ((++mCycle) >= 341)
//     {
//         mCycle = 0;
//         mScanLine++;

//         if (mScanLine >= 261)
//         {
//             ppustat.VBlank = 0;
//             mWin->flush();
//             mScanLine = -1;
//         }

//         // if (0<=mScanLine && mScanLine<261)
//         // {
//         //     uword base = 0x2000 + 0x400*ppuctl.NameTabSel;
//         //     drawNameTableRow(win, base, mScanLine);
//         // }
//     }
// }


union AttrCell {
    ubyte byte;
    struct {
        ubyte tl :2;
        ubyte tr :2;
        ubyte bl :2;
        ubyte br :2;
    } __attribute__((packed));
};


void Ricoh2C02::_quik( int nCycles )
{
    // uword base   = 0x2000 + 0x400*ppuctl.NameTabSel;
    auto *ntab   = (NTable*)(&mVRAM[0x400*ppuctl.NameTabSel]);
    ubyte bgSel  = (ppuctl.BgTileSel) ? 1 : 0;
    ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;

    for (int i=0; i<nCycles; i++)
    {
        ubyte y = ubyte(mScanLine);
        ubyte x = ubyte((mCycle-1) + i);

        ubyte row = y / 8;
        ubyte col = x / 8;
        // ubyte patternIdx = rdbus(base + 32*row + col);
        // ubyte paletteIdx = rdbus(base + 960 + 8*row + col);
        ubyte patternIdx = ntab->tile[row][col];
        ubyte paletteIdx = ntab->attr[row/4][col/4];

        // ubyte tileIdx = rdbus(base + 32*nameRow + nameCol);
        uword addr   = 0x1000*bgSel + 16*patternIdx + y%8;
        AttrCell lsb = { rdbus(addr+0) };
        AttrCell msb = { rdbus(addr+8) };

        ubyte pxl = 0;

        if ( (x%8<4) &&  (y%8<4)) { pxl = (msb.tl << 4) | lsb.tl; }
        if ( (x%8<4) && !(y%8<4)) { pxl = (msb.bl << 4) | lsb.bl; }
        if (!(x%8<4) &&  (y%8<4)) { pxl = (msb.tr << 4) | lsb.tr; }
        if (!(x%8<4) && !(y%8<4)) { pxl = (msb.br << 4) | lsb.br; }

        // ubyte plane0 = rdbus(addr+0);
        // ubyte plane1 = rdbus(addr+8);
        // ubyte bit = 7 - (x%8);
        // ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);
        ubyte off = mPaletteCtl[4*paletteIdx + pxl] & 0x3F;

        mWin->pixel(x, y, &mPalette[3*off]);
        // mWin->pixel(x, y, patternIdx, patternIdx, patternIdx);
        // mWin->pixel(x, y, paletteIdx, paletteIdx, paletteIdx);
    }
}


void Ricoh2C02::_drawPattern( int dstx, int dsty, ubyte bgTile, ubyte row, ubyte col )
{
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int y = 16*row + i;
            int x = 16*col + j;
            int tidx = 16*row + col;

            uword addr   = 0x1000*bgTile + 16*tidx + y%8;
            ubyte plane0 = rdbus(addr+0);
            ubyte plane1 = rdbus(addr+8);

            ubyte bit = 7 - (x % 8);
            ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);

            size_t dstidx = mPitch*((dsty+i)%240) + mBPP*((dstx+j)%256);
            ubyte  srcidx = 3 * (mPaletteCtl[4*mPalNo + pxl] & 0x3F);

            mWin->pixel(dstx+j, dsty+i, &mPalette[srcidx]);
            // memcpy(mFramebuffer+dstidx, mPalette+srcidx, 3);
            // ubyte *dst = &mFramebuffer[dstidx];
            // ubyte *src = &mPalette[srcidx];

            // dst[0] = src[0];
            // dst[1] = src[1];
            // dst[2] = src[2];
        }
    }
}


void Ricoh2C02::_drawNameTableRow( ubyte row )
{
    // uword base   = 0x2000 + 0x400*ppuctl.NameTabSel;
    uword base   = 0x400*ppuctl.NameTabSel;
    ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
    ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;

    for (uword col=0; col<32; col+=1)
    {
        ubyte idx = mVRAM[base + 32*row+col];
        _drawPattern(8*col, 8*row, bgTile, idx/16, idx%16);
    }
}


void Ricoh2C02::_drawNameTableCell( ubyte row, ubyte col )
{
    // uword base   = 0x2000 + 0x400*ppuctl.NameTabSel;
    uword base   = 0x400*ppuctl.NameTabSel;
    ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
    ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;

    // for (uword col=0; col<32; col+=1)
    // {
        ubyte idx = mVRAM[base + 32*row+col];
        _drawPattern(8*col, 8*row, bgTile, idx/16, idx%16);
    // }
}


void Ricoh2C02::reset()
{
    ppuctl  = {0};
    ppumask = {0};
    ppustat = {0};
    oamaddr = 0;
    oamdata = 0;
    SCROLL  = 0;
    ppuaddr = 0;
    ppudata = 0;

    mCycle    = 0;
    mScanLine = 0;
}


void Ricoh2C02::loadPalette( const std::string &path )
{
    size_t res = loadFileRaw(path, mPalette, sizeof(mPalette));
    LogAsrt(res==sizeof(mPalette), "Error loading .pal file \"%s\"\n", path.c_str());
}



// uint8_t Ricoh2C02::read2002()
// {
//     // printf("[read2002] vblank cleared\n");
//     uint8_t res = ppustat.byte;
//     ppustat.VBlank = 0;
//     ppuAddrLatch = true;
//     return res;
// }

// uint8_t Ricoh2C02::read2007()
// {
//     ubyte res = ppudata;

//     ppudata = rdbus(ppuaddr);
//     if (ppuaddr >= 0x3F00)
//         res = ppudata;
//     ppuaddr += (ppuctl.Increment) ? 32 : 1;
//     ppuaddr &= 0x3FFF;

//     return res;
// }

// void Ricoh2C02::write2005( uint8_t data )
// {

// }

// void Ricoh2C02::write2006( uint8_t data )
// {
//     switch (ppuAddrLatch)
//     {
//         case true:  ppuaddr = (ppuaddr & 0x00FF) | (uword(data) << 8); break;
//         case false: ppuaddr = (ppuaddr & 0xFF00) | (uword(data) << 0); break;
//     }
//     ppuAddrLatch = !ppuAddrLatch;
// }

// void Ricoh2C02::write2007( uint8_t data )
// {
//     wtbus(ppuaddr, data);
//     ppuaddr += (ppuctl.Increment) ? 32 : 1;
//     ppuaddr &= 0x3FFF;
// }
