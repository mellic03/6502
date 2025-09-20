#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>

using namespace memu;


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


/*
    https://austinmorlan.com/posts/nes_rendering_overview/

    The background is made up of a grid of tiles, each tile being 8x8 pixels.
    A frame is 256x240 pixels or 32x30 tiles.

    The nametable describes the layout of the frame’s background. The tiles
    start at (00,00) in the upper left and increase downward to the right
    until the final tile in the bottom right at (1F, 1D).


    https://www.nesdev.org/wiki/PPU_rendering
    - The PPU renders 262 scanlines per frame.
    - Each scanline lasts for 341 PPU clock cycles 
*/

size_t Ricoh2C02::tick()
{
    while (mAccum >= 341)
    {
        mScanline += 1;
        mAccum -= 341;
    }

    mAccum += 1;

    return 0;
}


void Ricoh2C02::reset()
{

}


void Ricoh2C02::loadPalette( const std::string &path )
{
    size_t bufsz = sizeof(mPalette);
    size_t res = loadFileRaw(path, mPalette, bufsz);
    LogAsrt(res==bufsz, "Failed reading .pal file \"%s\"\n", path.c_str());
}


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


    Attribute Tables
    --------------------------------------------------------------------------------------------
    The NES has four logical nametables, arranged in a 2x2 pattern. Each occupies a 1 KiB
    chunk of PPU address space, starting at $2000 at the top left, $2400 at the top right,
    $2800 at the bottom left, and $2C00 at the bottom right. 

    - An attribute table is a 64-byte array at the end of each nametable
      that controls which palette is assigned to each part of the background.

    - Each attribute table, starting at $23C0, $27C0, $2BC0, or $2FC0,
      is arranged as an 8x8 byte array:

            | 2xx0    2xx1    2xx2    2xx3    2xx4    2xx5    2xx6    2xx7
            ,-------+-------+-------+-------+-------+-------+-------+-------.
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xC0:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xC8:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xD0:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xD8:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xE0:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xE8:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
    2xF0:   | - + - | - + - | - + - | - + - | - + - | - + - | - + - | - + - |
            |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            +-------+-------+-------+-------+-------+-------+-------+-------+
    2xF8:   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |   .   |
            `-------+-------+-------+-------+-------+-------+-------+-------'

*/
