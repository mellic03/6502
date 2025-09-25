#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
#include <memu/pinout.hpp>
#include <cstring>

using namespace memu;


/*

    | Addr      | Size | Desc        | Mapped By |
    | ----------|------|-------------|-----------|
    | 0000-0FFF | 1000 | PtrnTable 0 | Cartridge |
    | 1000-1FFF | 1000 | PtrnTable 1 | Cartridge |
    | 2000-23BF | 03C0 | NameTable 0 | Cartridge |
    | 23C0-23FF | 0040 | AttrTable 0 | Cartridge |
    | 2400-27BF | 03c0 | Nametable 1 | Cartridge |
    | 27C0-27FF | 0040 | AttrTable 1 | Cartridge |
    | 2800-2BBF | 03c0 | Nametable 2 | Cartridge |
    | 2BC0-2BFF | 0040 | AttrTable 2 | Cartridge |
    | 2C00-2FBF | 03c0 | Nametable 3 | Cartridge |
    | 2FC0-2FFF | 0040 | AttrTable 3 | Cartridge |
    ----------------------------------------------

    The NES has 2kB of RAM dedicated to the PPU, usually mapped to the
    nametable address space from $2000-$2FFF, but this can be rerouted
    through custom cartridge wiring. The mappings above are the addresses
    from which the PPU uses to fetch data during rendering. The actual
    devices that the PPU fetches pattern, name table and attribute table
    data from is configured by the cartridge.

    0000-1FFF normally mapped by the cartridge to a CHR-ROM or CHR-RAM,
    often with a bank switching mechanism.

    2000-2FFF normally mapped to the 2kB NES internal VRAM, providing 2
    nametables with a mirroring configuration controlled by the cartridge,
    but it can be partly or fully remapped to ROM or RAM on the cartridge,
    allowing up to 4 simultaneous nametables.

    3000-3EFF usually a mirror of the 2kB region from $2000-2EFF. The PPU
    does not render from this address range, so this space has negligible
    utility.

    3F00-3FFF not configurable, always mapped to the internal palette control.
*/
#include <ctime>
#include <cstdio>


Ricoh2C02::Ricoh2C02(AddrSpace &bus)
:   HwModule(bus), BaseHw(),
    mCycle(0), mScanLine(0), mPrevLine(0),
    mPpuAddr(0), mPpuData(0)
{
    static constexpr ubyte blue   [] = { 0x11, 32, 0x36, 203, 79, 33, 128, 219, 80, 179, 4, 36, 25, 35, 183, 220, 46, 160, 80, 142, 25, 43, 6, 107, 214, 14, 247, 240, 149, 61, 225, 176 };
    static constexpr ubyte pink   [] = { 0x11, 246, 108, 92, 187, 42, 210, 95, 168, 78, 142, 98, 129, 239, 103, 87, 172, 13, 30, 101, 140, 87, 215, 181, 232, 143, 132, 245, 39, 10, 61, 155 };
    static constexpr ubyte purple [] = { 0x11, 227, 188, 126, 51, 108, 242, 50, 62, 34, 86, 167, 40, 63, 8, 141, 51, 132, 202, 219, 88, 130, 97, 206, 73, 196, 175, 197, 55, 172, 109, 187 };
    static constexpr ubyte luigi  [] = { 0x11, 89, 54, 254, 96, 12, 40, 67, 229, 115, 37, 146, 57, 101, 190, 224, 85, 64, 203, 98, 106, 182, 27, 196, 254, 231, 87, 157, 17, 71, 189, 134 };
    static constexpr ubyte ginger [] = { 0x11, 16, 22, 214, 245, 224, 70, 255, 64, 116, 122, 27, 99, 72, 85, 139, 182, 112, 82, 127, 92, 88, 47, 23, 185, 17, 132, 75, 47, 215, 16, 147 };

    memcpy(mPaletteCtl.data(), blue, mPaletteCtl.size());
}



/*
    https://www.nesdev.org/wiki/PPU_frame_timing

    Each PPU frame is 341*262=89342 PPU clocks long.

*/

// void Ricoh2C02::tick( EmuFramebuffer *fb )
// {
//     if (-1<=mScanLine && mScanLine<240)
//     {
//         if (mScanLine==0 && mCycle==0)
// 		{   // "Odd Frame" cycle skip
// 			mCycle = 1;
// 		}

// 		if (mScanLine==-1 && mCycle==1)
// 		{   // Effectively start of new frame, so clear vertical blank flag
// 			ppustat.VBlank = 0;
// 		}

//     }

//     // if (mScanDot >= 341)
//     // {
//     //     mScanLine = (mScanLine+1) % 262;
//     //     mScanDot = 0;
//     // }

//     // if (mScanLine==241 & mScanDot==1)
//     // {
//     //     ppustat.VBlank = 1;
//     //     ioWrite(ioINT, 1);
//     // }

//     // if (mScanLine==260)
//     // {
//     //     ppustat.VBlank = 0;
//     // }

// 	if (241<=mScanLine && mScanLine<261)
// 	{
// 		if (mScanLine==241 && mCycle==1)
// 		{   // Effectively end of frame, so set vertical blank flag
// 			ppustat.VBlank = 1;

//             // printf("NMIEnabled=%u\n", ppuctl.NMIEnabled);
            
//             if (ppuctl.NMIEnabled)
//             {
//                 ioWrite(ioINT, 1);
//             }
// 		}
// 	}

//     if (ppumask.BackGndEnable)
//     {
//         // do stuff
//     }


//     if (0<=lcurr && lcurr<240)
//     {
//         uword base = 0x2000 + 0x400*ppuctl.NameTabSel;
//         drawNameTableRow(fb, base, lcurr);
//     }


//     mCycle++;

// 	if (mCycle >= 341)
// 	{
// 		mCycle = 0;
// 		mScanLine++;

// 		if (mScanLine >= 261)
// 		{
// 			mScanLine = -1;
// 		}
// 	}

// }


void Ricoh2C02::reset()
{
    ppuctl  = {0};
    ppumask = {0};
    ppustat = {0};
    OAMADDR = 0;
    OAMDATA = 0;
    SCROLL  = 0;
    ADDR    = 0;
    DATA    = 0;

    mCycle    = 0;
    mScanLine = 0;

    mPpuAddr.reset();
    mPpuAddr.value = 0;

}


void Ricoh2C02::loadPalette( const std::string &path )
{
    size_t res = loadFileRaw(path, mPalette.data(), mPalette.size());
    LogAsrt(res==mPalette.size(), "Error loading .pal file \"%s\"\n", path.c_str());

    // std::vector<ubyte> temp = loadFileRaw(path);
    // LogAsrt(temp.size() == 192, "Error loading .pal file \"%s\"\n", path.c_str());
    // memcpy(mPalette, temp.data(), temp.size());
}

