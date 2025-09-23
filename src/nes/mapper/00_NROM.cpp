#include "types.hpp"
#include <memu/rwx.hpp>
#include <memu/log.hpp>
#include <cstdio>


/*
    - PRG ROM size: 16 KiB for NROM-128, 32 KiB for NROM-256 (DIP-28 standard pinout)
    - PRG ROM bank size: Not bankswitched
    - PRG RAM: 2 or 4 KiB, not bankswitched, only in Family Basic (but most emulators provide 8)
    - CHR capacity: 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
    - CHR bank size: Not bankswitched, see CNROM
    - Nametable mirroring: Solder pads select vertical or horizontal mirroring
    - Subject to bus conflicts: Yes, but irrelevant

    CPU $6000-$7FFF:
        Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB
        window, write protectable with an external switch.

    CPU $8000-$BFFF:
        First 16 KB of ROM.

    CPU $C000-$FFFF:
        Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).








    PICTURE PROCESSING UNIT
    --------------------------------------------------------------------------------------------

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
    |--------------------------------------------|

    -   The NES has 2kB of RAM dedicated to the PPU, usually mapped to the
        nametable address space from $2000-$2FFF, but this can be rerouted
        through custom cartridge wiring. The mappings above are the addresses
        from which the PPU uses to fetch data during rendering. The actual
        devices that the PPU fetches pattern, name table and attribute table
        data from is configured by the cartridge.

    -   0000-1FFF normally mapped by the cartridge to a CHR-ROM or CHR-RAM,
        often with a bank switching mechanism.

    -   2000-2FFF normally mapped to the 2kB NES internal VRAM, providing 2
        nametables with a mirroring configuration controlled by the cartridge,
        but it can be partly or fully remapped to ROM or RAM on the cartridge,
        allowing up to 4 simultaneous nametables.

    -   3000-3EFF usually a mirror of the 2kB region from $2000-2EFF. The PPU
        does not render from this address range, so this space has negligible
        utility.

    -   3F00-3FFF not configurable, always mapped to the internal palette control.
    --------------------------------------------------------------------------------------------

*/


using namespace NesEmu;


Mapper000_NROM::Mapper000_NROM( NesEmu::System &nes, GamePak *gpak )
{
    using namespace memu;

    auto *fh = (iNES_File*)(gpak->data());
    ubyte *fbeg = gpak->data();
    ubyte *fend = fbeg + gpak->size();
    ubyte *fpos = fbeg + 0x10;

    {
        syslog log("Mapper000_NROM");
        log("PrgRomNo16K:   %u", fh->PrgRomNo16K);
        log("ChrRomNo8K:    %u", fh->ChrRomNo8K);
        log("BankNo8K:      %u", fh->BankNo8K);
        log("PAL/NTSC:      %s", (fh->IsPAL) ? "PAL" : "NTSC");
    }

    mPrgRam = {new ubyte[8*1024], 8*1024};

    mPrgRom = {fpos, fh->PrgRomNo16K * 16*1024};
    fpos += mPrgRom.size;

    mChrRom = {fpos, fh->ChrRomNo8K * 8*1024};
    fpos += mChrRom.size;

    // CPU Mapping
    {
        auto &bus = nes.mBusCPU;

        // CPU --> PRG RAM
        bus.mapRWRange(0x6000, 0x7FFF, mPrgRam.base, mPrgRam.size);

        // CPU --> PRG ROM
        bus.mapRdRange(0x8000, 0xFFFF, mPrgRom.base, mPrgRom.size);
    }

    // PPU Mapping
    {
        auto  &bus    = nes.mBusPPU;
        auto  &ppu    = nes.mPPU;
        ubyte *ppuram = ppu.mVRAM.data();
        size_t ppursz = ppu.mVRAM.size();

        // PPU --> CHR ROM
        bus.mapRdRange(0x0000, 0x1FFF, mChrRom.base, mChrRom.size);

        // PPU --> PPU VRAM
        bus.mapRWRange(0x2000, 0x2FFF, ppuram, ppursz, ppursz-1);
        bus.mapRWRange(0x3000, 0x3EFF, ppuram, ppursz, ppursz-1);

        // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 3FFF
        bus.mapRWRange(0x3F00, 0x3FFF, ppu.mPaletteCtl, sizeof(ppu.mPaletteCtl));

    }

}




