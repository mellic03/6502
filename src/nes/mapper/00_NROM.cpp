#include "hwmapper.hpp"
using namespace NesEmu;



/*
    - PRG ROM size: 16 KiB for NROM-128, 32 KiB for NROM-256 (DIP-28 standard pinout)
    - PRG ROM bank size: Not bankswitched
    - PRG RAM: 2 or 4 KiB, not bankswitched, only in Family Basic (but most emulators provide 8)
    - CHR capacity: 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
    - CHR bank size: Not bankswitched, see CNROM
    - Nametable mirroring: Solder pads select vertical or horizontal mirroring
    - Subject to bus conflicts: Yes, but irrelevant
*/


/*
    CPU $6000-$7FFF:
        Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB
        window, write protectable with an external switch.

    CPU $8000-$BFFF:
        First 16 KB of ROM.

    CPU $C000-$FFFF:
        Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
*/

static void MapCpuBus( System &nes )
{
    using namespace NesFile;

    auto  &bus    = nes.mBusCPU;
    auto  *gpak   = nes.mGPak;
    auto  *fh     = gpak->mFile;
    ubyte *prgROM = gpak->mPrgROM.data<ubyte>();
    ubyte *chrROM = gpak->mChrROM.data<ubyte>();

    // CPU --> PRG ROM,  First 16KB of ROM
    bus.mapRange(0x8000, 0xBFFF, 0xBFFF-0x8000, prgROM, RWX::R);

    // CPU --> PRG ROM,  Mirror of 8000-BFFF (NROM-128) or final 16KB of ROM (NROM-256)
    uint16_t off = (fh->prgRomSz == 2) ? 0x4000 : 0x0000;
    bus.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, prgROM+off, RWX::R);

}

static void MapPpuBus( System &nes )
{
    using namespace NesFile;

    auto  &bus  = nes.mBusPPU;
    auto  &ppu  = nes.mPPU;
    auto  *gpak = nes.mGPak;

    printf("[MapPpuBus] gpak->mChrROM: %luK\n", gpak->mChrROM.size());
    ubyte *chrROM = gpak->mChrROM.data<ubyte>();
    // ubyte *prgRAM = gpak->mPrgRAM.data<ubyte>();


    // | Addr      | Size | Desc        | Mapped By |
    // | ----------|------|-------------|-----------|
    // | 0000-0FFF | 1000 | PtrnTable 0 | Cartridge |
    // | 1000-1FFF | 1000 | PtrnTable 1 | Cartridge |
    // | 2000-23BF | 03C0 | NameTable 0 | Cartridge |
    // | 23C0-23FF | 0040 | AttrTable 0 | Cartridge |
    // | 2400-27BF | 03c0 | Nametable 1 | Cartridge |
    // | 27C0-27FF | 0040 | AttrTable 1 | Cartridge |
    // | 2800-2BBF | 03c0 | Nametable 2 | Cartridge |
    // | 2BC0-2BFF | 0040 | AttrTable 2 | Cartridge |
    // | 2C00-2FBF | 03c0 | Nametable 3 | Cartridge |
    // | 2FC0-2FFF | 0040 | AttrTable 3 | Cartridge |
    // ----------------------------------------------

    // starting at $2000 at the top left, $2400 at the top right,
    // $2800 at the bottom left, and $2C00 at the bottom right.

    // PPU --> ChrROM
    bus.mapRange(0x0000, 0x1FFF, 0x1FFF, chrROM, RWX::R);

    // // PPU --> ChrRAM
    // bus.mapRange(0x1000, 0x1FFF, 0x0FFF, chrRAM);

}


void HwMapper00_NROM( System &nes )
{
    using namespace NesFile;
    MapCpuBus(nes);
    MapPpuBus(nes);
}
