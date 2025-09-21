#include <memu/hw/ricoh/2C02.hpp>
#include <memu/addrspace.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
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



Ricoh2C02::Ricoh2C02(AddrSpace &bus)
:   HwModule(bus), BaseHw(),
    // mPalette(new ubyte[192]),
    // mPaletteCtl(new ubyte[32]),
    mAccum(0), mScanline(0)
{
    // uint8_t *ram = mVRAM.data();
    // size_t   rsz = mVRAM.size();

    // // PPU --> PPU VRAM
    // bus.mapRange(0x2000, 0x2FFF, RWX_RW, ram, rsz);
    // bus.mapRange(0x3000, 0x3EFF, RWX_RW, ram, rsz);

    // // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 3FFF
    // // bus.mapRange(0x3F00, 0x3FFF, RWX_RW, mPaletteCtl, sizeof(mPaletteCtl));
    // bus.mapRange(0x3F00, 0x3FFF, RWX_RW, mPalette, sizeof(mPalette));
}


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
    // for (int i=0; i<32; i++)
    // {
    //     mPaletteCtl[i] = 0x0F;
    // }
}


void Ricoh2C02::loadPalette( const std::string &path )
{
    std::vector<ubyte> temp = loadFileRaw(path);

    LogAsrt(temp.size() == 192, "Error loading .pal file \"%s\"\n", path.c_str());
    // memcpy(mPalette, temp.data(), temp.size());

    for (int i=0; i<64; i++)
    {
        mPalette[i].r = temp[3*i + 0];
        mPalette[i].g = temp[3*i + 1];
        mPalette[i].b = temp[3*i + 2];
    }

    // size_t bufsz = sizeof(mPalette);
    // size_t rawsz = loadFileRaw(path, mPalette, bufsz);
    // LogAsrt(rawsz==bufsz, "Failed to load .pal file \"%s\"\n", path.c_str());
}

