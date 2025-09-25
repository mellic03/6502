#include <memu/nes/ppu.hpp>
#include <memu/nes/nes.hpp>
#include <memu/addrspace.hpp>
#include <memu/display.hpp>
#include <memu/pinout.hpp>






void NesPPU::tick()
{
    if (memu::ioRead(ioRES) == 0)
    {
        this->reset();
    }

    // if (onRange(0, 240))
    // {
    //     uword base = 0x2000 + 0x400*ppuctl.NameTabSel;
    //     _drawNameTableRow(ubyte(mScanLine/8));
    // }

    if (mScanLine==241 && mCycle==1)
    {
        ppustat.VBlank = 1;
    
        // if (ppuctl.NMIEnabled)
        {
            memu::ioWrite(ioINT, 1);
        }
    }


    if ((++mCycle) >= 341)
    {
        mCycle = 0;
        mScanLine++;

        if (mScanLine >= 261)
        {
            ppustat.VBlank = 0;
            _entire_frame();
            mWin->flush();
            mScanLine = -1;
        }

        // if (0<=mScanLine && mScanLine<261)
        // {
        //     uword base = 0x2000 + 0x400*ppuctl.NameTabSel;
        //     drawNameTableRow(win, base, mScanLine);
        // }
    }

}



void NesPPU::_entire_tile( int x0, int y0, uword tidx, uword palIdx )
{
    printf("%u\n", tidx);
    // ubyte bgSel = (ppuctl.BgTileSel) ? 1 : 0;

    for (int y=0; y<8; y++)
    {
        // uword base = 0x400*bgSel + 16*tidx + y%8;
        uword base = 16*tidx + y%8;
        ubyte lo   = rdbus(base + 0);
        ubyte hi   = rdbus(base + 8);

        for (int x=0; x<8; x++)
        {
            ubyte bit = 7 - (x % 8);
            ubyte lsb = (lo >> bit) & 0x01;
            ubyte msb = (hi >> bit) & 0x01;
            ubyte pxl = msb + lsb;
            ubyte off = mPaletteCtl[4*palIdx + pxl] & 0x3F;
            // mWin->pixel(x, y, &mPalette[3*off]);

            ubyte val = 63*pxl;
            mWin->pixel(x0+x, y0+y, val, val, val);

            // ubyte val = 4 * (8*y + x);
            // ubyte val = 4 * (x*y);
            // mWin->pixel(x0+x, y0+y, 32*x, 32*y, 255);
        }
    }
}


void NesPPU::_entire_frame()
{
    uword base = 0x2000 + 0x400*ppuctl.NameTabSel;

    for (uword row=0; row<30; row++)
    {
        for (uword col=0; col<32; col++)
        {
            ubyte patIdx = rdbus(base + 32*row + col);
            ubyte palIdx = 0; // rdbus(base + 960 + ... );
            _entire_tile(8*col, 8*row, patIdx, palIdx);
        }
    }

}



void NesPPU::drawPattern( EmuFramebuffer *fb, int dstx, int dsty, ubyte bgTile, ubyte row, ubyte col )
{
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int y = 16*row + i;
            int x = 16*col + j;

            // Compute tile index
            int tidx = 16*row + col;

            // Address into CHR
            uword addr   = 0x1000*bgTile + 16*tidx + y%8;
            ubyte plane0 = rdbus(addr+0);
            ubyte plane1 = rdbus(addr+8);

            ubyte bit = 7 - (x % 8);
            ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);

            // // if (NesEmu::CONF["debug"]["GreyPalettes"])
            // {
            //     fb->pixel(dx+j, dy+i, 62*pxl, 62*pxl, 62*pxl);
            //     continue;
            // }

            ubyte off = mPaletteCtl[4*mPalNo + pxl] & 0x3F;
            fb->pixel(dstx+j, dsty+i, &mPalette[3*off]);
        }
    }
}

ubyte NesPPU::readNameTile( uword base, ubyte row, ubyte col )
{
    return rdbus(base + 32*row + col);
}


void NesPPU::drawNameTableCell( EmuFramebuffer *fb, uword base, ubyte row, ubyte col )
{
    ubyte idx    = rdbus(base + 32*row+col);
    ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
    ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;
    drawPattern(fb, 8*col, 8*row, bgTile, idx/16, idx%16);
}


void NesPPU::drawNameTableRow( EmuFramebuffer *fb, uword base, ubyte row )
{
    // uword off = 0;

    // for (uword row=0; row<30; row+=1)
    // {
    //     if (row != r)
    //     {
    //         off += 32;
    //         continue;
    //     }

    //     for (uword col=0; col<32; col+=1)
    //     {
    //         ubyte idx    = rdbus(base+off);
    //         ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
    //         ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;
    //         drawPattern(fb, 8*col, 8*row, bgTile, idx/16, idx%16);
    //         off += 1;
    //     }
    // }

    uword off = 32*row;

    for (uword col=0; col<32; col+=1)
    {
        ubyte idx    = rdbus(base+off + col);
        ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
        ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;
        drawPattern(fb, 8*col, 8*row, bgTile, idx/16, idx%16);
        // off += 1;
    }

}


void NesPPU::drawNameTable( EmuFramebuffer *fb, uword base )
{
    uword off = 0;

    for (uword row=0; row<30; row+=1)
    {
        for (uword col=0; col<32; col+=1)
        {
            ubyte idx    = rdbus(base+off);
            ubyte bgTile = (ppuctl.BgTileSel) ? 1 : 0;
            ubyte spTile = (ppuctl.SpriteTileSel) ? 1 : 0;
            drawPattern(fb, 8*col, 8*row, bgTile, idx/16, idx%16);
            off += 1;
        }
    }
}


// /*
//     Register	        At Power    After Reset
//     PPUCTRL             0000 0000	0000 0000
//     PPUMASK             0000 0000	0000 0000
//     PPUSTATUS           +0+x xxxx	U??x xxxx
//     OAMADDR             $00	        unchanged
//     $2005/$2006 latch	cleared     cleared
//     PPUSCROLL           $0000	    $0000
//     PPUADDR             $0000	    unchanged
//     PPUDATA             read buffer	$00	$00

//     ? = unknown, x = irrelevant, + = often set, U = unchanged
// */


// /*
//     - Horizontal arrangement: $2000 and $2800 contain the first nametable,
//       and $2400 and $2C00 contain the second nametable (e.g. Super Mario Bros.),
//       accomplished by connecting CIRAM A10 to PPU A10.

//     - Vertical arrangement: $2000 and $2400 contain the first nametable,
//       and $2800 and $2C00 contain the second nametable (e.g. Kid Icarus),
//       accomplished by connecting CIRAM A10 to PPU A11.
// */
