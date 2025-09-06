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
void HwMapper00_NROM::map( System &nes, GamePak *cart )
{
    using namespace NesFile;

    auto &bus0 = nes.mBusCPU;
    auto &bus1 = nes.mBusPPU;
    auto &ppu  = nes.mPPU;
    auto *pak  = nes.mGamePak;

    iNES *file  = pak->miNES;
    auto *pgrom = file->mPrgROM.data();
    auto *chrom = file->mChrROM.data();
    auto &H     = file->mHead;

    // CPU --> PRG ROM,  First 16KB of ROM
    bus0.mapRange(0x8000, 0xBFFF, 0xBFFF-0x8000, RWX::RW, pgrom);

    // CPU --> PRG ROM,  Mirror of 8000-BFFF (NROM-128) or final 16KB of ROM (NROM-256)
    if (H.prgsz==2) bus0.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, RWX::RW, pgrom + 0x4000);
    else            bus0.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, RWX::RW, pgrom);


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

    // PPU --> CHR ROM
    bus1.mapRange(0x0000, 0x0FFF, 0x0FFF, RWX::R, chrom);
    bus1.mapRange(0x1000, 0x1FFF, 0x0FFF, RWX::R, chrom);

    // PPU --> NameTables

    // 0x2000: 0b00
    // 0x2400: 0b01
    // 0x2800: 0b10
    // 0x2C00: 0b11

    bus1.mapRange(0x2000, 0x23FF, 1024-1, RWX::RW, &(ppu.mTables[0]));
    bus1.mapRange(0x2400, 0x27FF, 1024-1, RWX::RW, &(ppu.mTables[1]));
    bus1.mapRange(0x2800, 0x2BFF, 1024-1, RWX::RW, &(ppu.mTables[2]));
    bus1.mapRange(0x2C00, 0x2FFF, 1024-1, RWX::RW, &(ppu.mTables[3]));

    // starting at $2000 at the top left, $2400 at the top right,
    // $2800 at the bottom left, and $2C00 at the bottom right.
}
