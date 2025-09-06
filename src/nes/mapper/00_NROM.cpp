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

    auto &bus = nes.mBusCPU;
    auto *pak = nes.mGamePak;

    iNES *file  = pak->miNES;
    auto *pgrom = file->mPrgROM.data();
    auto &H     = file->mHead;

    // CPU --> ROM,  First 16KB of ROM
    bus.mapRange(0x8000, 0xBFFF, 0xBFFF-0x8000, RWX::RW, pgrom);

    // CPU --> ROM,  Mirror of 8000-BFFF (NROM-128) or final 16KB of ROM (NROM-256)
    if (H.prgsz==2) bus.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, RWX::RW, pgrom + 0x4000);
    else            bus.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, RWX::RW, pgrom);
}
