#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
#include <memu/pinout.hpp>

#include <cstdio>
#include <cstring>
#include <ctime>

using namespace memu;



Ricoh2C02::Ricoh2C02( AddrSpace &bus, EmuWindow *gamewin )
:   HwModule(bus),
    BaseHw(),
    mGameWin(gamewin),
    mScanLine(0),
    mCycle(0)
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
    // if (memu::ioRead(ioLineRES) == 0)
    // {
    //     this->reset();
    // }

    // if (0<=mScanLine && mScanLine<239)
    // {
    //     if (0<=mCycle && mCycle<256)
    //     {
    //         _single_pixel(ubyte(mCycle), ubyte(mScanLine));
    //     }
    // }

    if (mScanLine==241 && mCycle==1)
    {
        ppustat.VBlank = 1;
        // printf("VBLANK\n");

        if (ppuctl.NMIEnabled)
        {
            ioWrite(ioLineNMI, 1);
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
            ppustat.VBlank = 0;
            _entire_frame();
            mGameWin->flush();
            mScanLine = -1;
        }
    }
}


void Ricoh2C02::preRenderChrTile( EmuWindow *fb, int x0, int y0, uword tsel, uword tidx, uword pidx )
{
    uword base = 0x1000*tsel + 16*tidx;

    for (ubyte y=0; y<8; y++)
    {
        for (ubyte x=0; x<8; x++)
        {
            ubyte lsb  = rdbus(base + y+0);
            ubyte msb  = rdbus(base + y+8);
            ubyte lo   = (lsb >> (7-x)) & 0x01;
            ubyte hi   = (msb >> (7-x)) & 0x01;
            ubyte pxl  = (hi << 1) | lo;
            fb->frameBuffer()->pixel(x0+x, y0+y, 64*pxl);
        }
    }
}

void Ricoh2C02::preRenderChrRom( EmuWindow *fb )
{
    for (uword col=0; col<16; col++)
    {
        for (uword row=0; row<16; row++)
        {
            preRenderChrTile(fb, 8*col, 8*row + 0*128, 0, 16*row + col, 0);
            preRenderChrTile(fb, 8*col, 8*row + 1*128, 1, 16*row + col, 0);
        }
    }
}



// union PtrnAddr
// {
//     uword value;
//     struct {
//         uword y      :3;
//         uword plane  :1;
//         uword idx_lo :4;
//         uword idx_hi :4;
//         uword sel    :1;
//         uword zero   :3;
//     } __attribute__((packed));
// };


struct AttrAddr
{
    ubyte coarse_x: 3;
    ubyte coarse_y: 3;
    ubyte attr_off: 4;
    ubyte ntab_sel: 2;
    ubyte rest[4];
};

void Ricoh2C02::_single_pixel( ubyte x, ubyte y )
{
    uword ntab = 0x2000 + 0x0400*ppuctl.NameTabSel;
    ubyte tidx = rdbus(ntab + 32*(y/8) + x/8);

    uword ptab = 0x1000*ppuctl.BgTileSel + 16*tidx;
    ubyte lsb  = rdbus(ptab + (y%8)+0);
    ubyte msb  = rdbus(ptab + (y%8)+8);

    ubyte lo   = (lsb >> (7 - (x%8))) & 0x01;
    ubyte hi   = (msb >> (7 - (x%8))) & 0x01;
    ubyte pxl  = (hi << 1) | lo;

    ubyte attr = rdbus(ntab + 960 + 8*(y/32) + x/32);
    ubyte shft = 4*((y%32) / 16) + 2*((x%32) / 16);
    ubyte pidx = (attr >> shft) & 0x03;

    ubyte off = 0;
    if (pxl) off = rdbus(0x3F00 + 4*pidx + pxl) & 0x3F;
    else     off = rdbus(0x3F00) & 0x3F; // universal background

    mGameWin->frameBuffer()->pixel(x, y, &mPalette[3*off]);
}


void Ricoh2C02::_entire_tile( int x0, int y0, ubyte tidx, ubyte pidx )
{
    uword ptab = 0x1000*ppuctl.BgTileSel + 16*tidx;

    for (ubyte i=0; i<8; i++)
    {
        ubyte lsb = rdbus(ptab + i+0);
        ubyte msb = rdbus(ptab + i+8);

        for (ubyte j=0; j<8; j++)
        {
            ubyte lo  = (lsb & 128) >> 7;
            ubyte hi  = (msb & 128) >> 7;
            ubyte pxl = (hi << 1) | lo;
            lsb<<=1; msb<<=1;

            ubyte off = 0;
            if (pxl) off = rdbus(0x3F00 + 4*pidx + pxl) & 0x3F;
            else     off = rdbus(0x3F00) & 0x3F; // universal background

            mGameWin->frameBuffer()->pixel(x0+j, y0+i, &mPalette[3*off]);
        }
    }
}


void Ricoh2C02::_entire_frame()
{
    uword ntab = 0x2000 + 0x0400*ppuctl.NameTabSel;

    for (uword row=0; row<30; row++)
    {
        for (uword col=0; col<32; col++)
        {
            ubyte tidx = rdbus(ntab + 32*row + col);
            ubyte attr = rdbus(ntab + 960 + 8*((8*row)/32) + (8*col)/32);
            ubyte shft = 4*(((8*row)%32) / 16) + 2*(((8*col)%32) / 16);
            ubyte pidx = (attr >> shft) & 0x03;
            _entire_tile(8*col, 8*row, tidx, pidx);
        }
    }


    for (ubyte i=0; i<64; i++)
    {
        auto *S = (OamSprite*)(mOAM + 4*i);

        if (S->ymin==0 && S->tile==0 && S->attr==0 && S->xmin==0)
        {
            break;
        }

        uword bank = ppuctl.SpriteTileSel; // S->tile & 0b0000'0001;
        uword tidx = S->tile;
        uword pidx = (S->palette);
        ubyte x0 = S->xmin;
        ubyte y0 = S->ymin+1;

        for (ubyte i=0; i<8; i++)
        {
            ubyte lsb = rdbus(0x1000*bank + 16*tidx + i+0);
            ubyte msb = rdbus(0x1000*bank + 16*tidx + i+8);

            ubyte lo=0, hi=0, pxl=0;

            for (ubyte j=0; j<8; j++)
            {
                ubyte lo  = (lsb & 128) >> 7;
                ubyte hi  = (msb & 128) >> 7;
                ubyte pxl = (hi << 1) | lo;
                lsb<<=1; msb<<=1;

                if (pxl > 0)
                {
                    ubyte off = rdbus(0x3F00 + 4 + 4*pidx + pxl) & 0x3F;

                    ubyte x = (S->hflip) ? x0+7-j : x0+j;
                    mGameWin->frameBuffer()->pixel(x, y0+i, &mPalette[3*off]);
                }
            }
        }
    }
}


void Ricoh2C02::reset()
{
    mClock    = 0;

    ppuctl    = {0};
    ppumask   = {0};
    ppustat   = {0};
    oamaddr   = 0;
    oamdata   = 0;
    ppuscroll = 0;
    ppuaddr   = 0;
    ppudata   = 0;

    mScanLine = -1;
    mCycle    =  0;
}


void Ricoh2C02::flush()
{
    while (mScanLine != -1 && mCycle != 0)
    {
        this->tick();
    }
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
