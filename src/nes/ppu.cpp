#include <memu/nes/ppu.hpp>
#include <memu/nes/nes.hpp>
#include <memu/addrspace.hpp>
#include <memu/display.hpp>


void NesPPU::drawPatternTable( EmuFramebuffer *fb, int palNo, ivec2 dpos, ivec2 spos )
{
    for (int i=0; i<128; i++)
    {
        for (int j=0; j<128; j++)
        {
            int y = spos.y + i;
            int x = spos.x + j;

            // Compute tile index
            int tileX = x/8;
            int tileY = y/8;
            int tileIndex = 16*tileY + tileX;

            // Address into CHR
            int tileAddr = 16*tileIndex + y%8;
            ubyte plane0 = rdbus(tileAddr + 0);
            ubyte plane1 = rdbus(tileAddr + 8);

            ubyte bit = 7 - (x % 8);
            ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);

            // ubyte  off = rdbus(0x3F00 + 4*palNo + pxl);
            // ubyte  off = mPaletteCtl[4*palNo + pxl] & 0x3F;
            // ubyte *rgb = mPalette + 3*off;
            // fb->pixel(dpos.x+j, dpos.y+i, rgb);

            fb->pixel(dpos.x+j, dpos.y+i, 62*pxl, 62*pxl, 62*pxl);
        }
    }
}

void NesPPU::drawPatternTable( EmuFramebuffer *fb, int palNo, ivec2 spos )
{
    drawPatternTable(fb, palNo, {0, 0}, spos);
}



/*
    128x128 pixels --> 16x16 tiles
*/

void NesPPU::drawPattern( EmuFramebuffer *fb, int tabNo, int dx, int dy, int sx, int sy )
{
    static constexpr ubyte conf0  [] = { 0x11, 32, 0x36, 203, 79, 33, 128, 219, 80, 179, 4, 36, 25, 35, 183, 220, 46, 160, 80, 142, 25, 43, 6, 107, 214, 14, 247, 240, 149, 61, 225, 176 };
    static constexpr ubyte conf1  [] = { 0x11, 32, 144, 203, 79, 33, 128, 219, 80, 179, 4, 36, 25, 35, 183, 220, 46, 160, 80, 142, 25, 43, 6, 107, 214, 14, 247, 240, 149, 61, 225, 176 };

    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int y = 8*sy + i;
            int x = 8*sx + j;

            // Compute tile index
            int tx = x/8;
            int ty = y/8;
            int tidx = 16*ty + tx;

            // Address into CHR
            uword addr   = 0x1000*tabNo + 16*tidx + y%8;
            ubyte plane0 = rdbus(addr+0);
            ubyte plane1 = rdbus(addr+8);

            ubyte bit = 7 - (x % 8);
            ubyte pxl = ((plane0 >> bit) & 1) | (((plane1 >> bit) & 1) << 1);

            // // if (NesEmu::CONF["debug"]["GreyPalettes"])
            // {
            //     fb->pixel(dx+j, dy+i, 62*pxl, 62*pxl, 62*pxl);
            //     continue;
            // }

            auto &ctl = (tabNo==0) ? conf0 : conf1;
            ubyte  off = ctl[4*mPalNo + pxl] & 0x3F;
            fb->pixel(dx+j, dy+i, mPalette + 3*off);
        }
    }
}


ubyte NesPPU::readNameTile( uword base, ubyte row, ubyte col )
{
    return rdbus(base + 32*row + col);
}

/*
    Four 1K tables from 0x2000.
    0x2000, 0x2400, 0x2800, 0x2C00
*/

void NesPPU::drawNameTable( EmuFramebuffer *fb, uword base )
{
    uword off = 0;

    for (uword y=0; y<240; y+=8)
    {
        for (uword x=0; x<256; x+=8)
        {
            // uword off = 256*y + x;
            ubyte val = rdbus(base+off);

            fb->pixel(x, y, val, val, val);
            // fb->pixel(x, y, 255, 255, 255);

            drawPattern(fb, 1, x, y, val%128, val/128);

            // for (int j=0; j<8; j++)
            // {
            //     for (int i=0; i<8; i++)
            //     {
            //         fb->pixel(x+j, y+i, val, val, val);
            //     }
            // }

            off += 1;
        }
    }
}

// void NesPPU::drawNameTable( EmuFramebuffer *fb, uword base, int dstx, int dsty )
// {
//     for (uword row=0; row<8; row++)
//     {
//         for (uword col=0; col<8; col++)
//         {
//             int y = 8*row;
//             int x = 8*col;

//             uword off = 8*y + x;
//             ubyte val = rdbus(base + off);

//             fb->pixel(dstx+x, dsty+y, val, val, val);
//         }
//     }
// }

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

// NesPPU::NesPPU( memu::AddrSpace &bus )
// :   HwModule(bus)
// {
//     mMMIO   = (RegMMIO*)(mVRAM.data() + 0);
//     mTables = (TableData*)(mVRAM.data() + sizeof(RegMMIO));

//     // ubyte *VRAM = mVRAM.data();
//     // mBus->mapRange(0x2000, 0x2FFF, 2048-1, &mTables[0]);

//     // | Addr      | Size | Desc        | Mapped By |
//     // | ----------|------|-------------|-----------|
//     // | 0000-0FFF | 1000 | PtrnTable 0 | Cartridge |
//     // | 1000-1FFF | 1000 | PtrnTable 1 | Cartridge |
//     // | 2000-23BF | 03C0 | NameTable 0 | Cartridge |
//     // | 23C0-23FF | 0040 | AttrTable 0 | Cartridge |
//     // | 2400-27BF | 03C0 | Nametable 1 | Cartridge |
//     // | 27C0-27FF | 0040 | AttrTable 1 | Cartridge |
//     // | 2800-2BBF | 03C0 | Nametable 2 | Cartridge |
//     // | 2BC0-2BFF | 0040 | AttrTable 2 | Cartridge |
//     // | 2C00-2FBF | 03C0 | Nametable 3 | Cartridge |
//     // | 2FC0-2FFF | 0040 | AttrTable 3 | Cartridge |
//     // ----------------------------------------------

//     /*
//         - The NES has 2kB of RAM dedicated to the PPU, usually mapped to the
//           nametable address space from $2000-$2FFF, but this can be rerouted
//           through custom cartridge wiring. The mappings above are the addresses
//           from which the PPU uses to fetch data during rendering. The actual
//           devices that the PPU fetches pattern, name table and attribute table
//           data from is configured by the cartridge.

//         - 0000-1FFF normally mapped by the cartridge to a CHR-ROM or CHR-RAM,
//           often with a bank switching mechanism.

//         - 2000-2FFF normally mapped to the 2kB NES internal VRAM, providing 2
//           nametables with a mirroring configuration controlled by the cartridge,
//           but it can be partly or fully remapped to ROM or RAM on the cartridge,
//           allowing up to 4 simultaneous nametables.

//         - 3000-3EFF usually a mirror of the 2kB region from $2000-2EFF. The PPU
//           does not render from this address range, so this space has negligible
//           utility.

//         - 3F00-3FFF not configurable, always mapped to the internal palette control.

//     */

//     // PPU --> NameTables
//     // mBus->mapRange(0x2000, 0x23FF, 1024-1, &mTables[0]);
// //     mBus->mapRange(0x2400, 0x27FF, 1024-1, &mTables[1]);
// //     mBus->mapRange(0x2800, 0x2BFF, 1024-1, &mTables[2]);
// //     mBus->mapRange(0x2C00, 0x2FFF, 1024-1, &mTables[3]);
// }


// void NesPPU::tick()
// {
//     return;
// }

// void NesPPU::reset()
// {
//     return;
// }

