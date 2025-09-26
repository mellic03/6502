#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
#include <memu/pinout.hpp>

#include <cstdio>
#include <cstring>
#include <ctime>

using namespace memu;



Ricoh2C02::Ricoh2C02( AddrSpace &bus, EmuWindow *gamewin, EmuWindow *chrwin )
:   HwModule(bus),
    BaseHw(),
    mGameWin(gamewin),
    mChrWin(chrwin),
    mCycle(0),
    mScanLine(0)
{
    this->reset();

    static constexpr ubyte blue   [] = { 0x11, 32, 0x36, 203, 79, 33, 128, 219, 80, 179, 4, 36, 25, 35, 183, 220, 46, 160, 80, 142, 25, 43, 6, 107, 214, 14, 247, 240, 149, 61, 225, 176 };
    static constexpr ubyte pink   [] = { 0x11, 246, 108, 92, 187, 42, 210, 95, 168, 78, 142, 98, 129, 239, 103, 87, 172, 13, 30, 101, 140, 87, 215, 181, 232, 143, 132, 245, 39, 10, 61, 155 };
    static constexpr ubyte purple [] = { 0x11, 227, 188, 126, 51, 108, 242, 50, 62, 34, 86, 167, 40, 63, 8, 141, 51, 132, 202, 219, 88, 130, 97, 206, 73, 196, 175, 197, 55, 172, 109, 187 };
    static constexpr ubyte luigi  [] = { 0x11, 89, 54, 254, 96, 12, 40, 67, 229, 115, 37, 146, 57, 101, 190, 224, 85, 64, 203, 98, 106, 182, 27, 196, 254, 231, 87, 157, 17, 71, 189, 134 };
    static constexpr ubyte ginger [] = { 0x11, 16, 22, 214, 245, 224, 70, 255, 64, 116, 122, 27, 99, 72, 85, 139, 182, 112, 82, 127, 92, 88, 47, 23, 185, 17, 132, 75, 47, 215, 16, 147 };

    memcpy(mPaletteCtl, blue, sizeof(mPaletteCtl));
}



void Ricoh2C02::tick()
{
    // if (memu::ioRead(ioRES) == 0)
    // {
    //     this->reset();
    // }

    if (mScanLine==241 && mCycle==1)
    {
        ppustat.VBlank = 1;
        // printf("VBLANK\n");

        if (ppuctl.NMIEnabled)
        {
            *ioLineNMI &= 0xFF;
        }
    }

    mClock++;
    mCycle++;

    if (mCycle >= 341)
    {
        mCycle = 0;
        mScanLine++;

        if (mScanLine >= 261)
        {
            _entire_frame();
            mGameWin->flush();
            mScanLine = -1;
        }
    }
}


void Ricoh2C02::preRenderChrTile( EmuWindow *fb, int x0, int y0, uword tidx, uword palIdx )
{
    ubyte bgSel = (ppuctl.BgTileSel) ? 1 : 0;

    for (int y=0; y<8; y++)
    {
        uword base = 0x1000*bgSel + 16*tidx;
        ubyte low  = rdbus(base + y+0);
        ubyte high = rdbus(base + y+8);

        for (int x=0; x<8; x++)
        {
            ubyte bit = 7 - (x % 8);
            ubyte lsb = (low  >> bit) & 0x01;
            ubyte msb = (high >> bit) & 0x01;
            ubyte pxl = msb + lsb;
            // ubyte off = mPaletteCtl[4*palIdx + pxl] & 0x3F;
            // mWin->pixel(x, y, &mPalette[3*off]);

            ubyte val = (255/3) * pxl;
            fb->frameBuffer()->pixel(x0+x, y0+y, val);

            // ubyte val = 4 * (8*y + x);
            // ubyte val = 4 * (x*y);
            // mWin->pixel(x0+x, y0+y, 32*x, 32*y, 255);
        }
    }
}


void Ricoh2C02::preRenderChrRom( EmuWindow *fb )
{
    // auto *ntab = (NTable*)(&mVRAM[0x400*ppuctl.NameTabSel]);
    uword base = 0x2000 + 0x400*ppuctl.NameTabSel;

    for (uword row=0; row<16; row++)
    {
        for (uword col=0; col<16; col++)
        {
            int idx = 16*row + col;
            preRenderChrTile(fb, 8*col, 8*row, idx, 0);
        }
    }
}



void Ricoh2C02::_entire_tile( int x0, int y0, uword tidx, uword palIdx )
{
    ubyte bgSel = (ppuctl.BgTileSel) ? 1 : 0;
    int srcy = 8*(tidx / 16);
    int srcx = 8*(tidx % 16);

    for (int y=0; y<8; y++)
    {
        uword base = 0x1000*bgSel + 16*tidx;
        ubyte low  = rdbus(base + y+0);
        ubyte high = rdbus(base + y+8);

        for (int x=0; x<8; x++)
        {
            // ubyte bit = 7 - (x % 8);
            // ubyte lsb = (low  >> bit) & 0x01;
            // ubyte msb = (high >> bit) & 0x01;
            // ubyte pxl = msb + lsb;
            // ubyte off = mPaletteCtl[4*mPalNo + pxl] & 0x3F;
            // mGameWin->frameBuffer()->pixel(x0+x, y0+y, &mPalette[3*off]);
            // // mGameWin->frameBuffer()->pixel(x0+x, y0+y, tidx);

            ubyte *C = mChrWin->frameBuffer()->getPixel(srcx+x, srcy+y);
            mGameWin->frameBuffer()->pixel(x0+x, y0+y, C);
        }
    }
}


void Ricoh2C02::_entire_frame()
{
    uword base = 0x2000 + 0x400*ppuctl.NameTabSel;

    for (uword row=0; row<30; row++)
    {
        for (uword col=0; col<32; col++)
        {
            int tileIdx = rdbus(base + 32*row + col);
            int palIdx = mPalNo; // rdbus(base + 960 + ... );
            _entire_tile(8*col, 8*row, tileIdx, palIdx);
        }
    }
}



void Ricoh2C02::_quik( int nCycles )
{
    // auto *ntab   = (NTable*)(&mVRAM[0x400*ppuctl.NameTabSel]);
    uword base   = 0x2000 + 0x400*ppuctl.NameTabSel;
    ubyte bgSel  = (ppuctl.BgTileSel) ? 1 : 0;
    ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;

    for (int i=0; i<nCycles; i++)
    {
        ubyte y = ubyte(mScanLine);
        ubyte x = ubyte((mCycle-1) + i);

        ubyte row = y / 8;
        ubyte col = x / 8;

        ubyte patternIdx = rdbus(base + 32*row + col);
        ubyte paletteIdx = rdbus(base + 960 + 8*row + col);
        // ubyte patternIdx = ntab->tile[row][col];
        // ubyte paletteIdx = ntab->attr[row/4][col/4];

        // ubyte tileIdx = rdbus(base + 32*nameRow + nameCol);
        uword addr = 0x1000*bgSel + 16*patternIdx + y%8;
        ubyte plane0 = rdbus(addr+0);
        ubyte plane1 = rdbus(addr+8);
        ubyte bit = 7 - (x%8);
        ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);

        ubyte off = mPaletteCtl[4*paletteIdx + pxl] & 0x3F;
        mGameWin->frameBuffer()->pixel(x, y, &mPalette[3*off]);

        // mWin->pixel(x, y, 12*patternIdx, 12*patternIdx, 12*patternIdx);
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
            ubyte  srcidx = 3 * (mPaletteCtl[4*mPalNo + pxl] & 0x3F);

            mGameWin->frameBuffer()->pixel(dstx+j, dsty+i, &mPalette[srcidx]);
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
