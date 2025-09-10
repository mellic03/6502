#include "types.hpp"
#include <memu/rwx.hpp>
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
*/
using namespace NesEmu;


Mapper000_NROM::Mapper000_NROM( NesEmu::System &nes, GamePak *gpak )
{
    // auto  *gpak = nes.mGPak;
    // ubyte *prgRom = gpak->mPrgROM.data();
    // ubyte *chrRom = gpak->mChrROM.data();

    auto *fh = (iNES_File*)(gpak->data());

    ubyte *fbeg = gpak->data();
    ubyte *fend = fbeg + gpak->size();
    ubyte *fpos = fbeg + 0x10;

    assert(fh->PrgRomNo16K <= MAX_PRG_BANKS);
    for (int i=0; i<fh->PrgRomNo16K; i++)
    {
        mPrgRom[i] = {fpos, 16*1024};
        fpos += mPrgRom[i].size;
    }

    if (fh->PrgRomNo16K <= 1)
    {
        mPrgRom[1] = mPrgRom[0];
    }

    assert(fh->ChrRomNo8K <= MAX_CHR_BANKS);
    for (int i=0; i<fh->ChrRomNo8K; i++)
    {
        mChrRom[i] = {fpos, 8*1024};
        fpos += mChrRom[i].size;
    }

    printf("Remaining: %lu\n",  fend - fpos);

    // CPU Mapping
    {
        using namespace memu;
        auto &bus = nes.mBusCPU;
        bus.mapRange(0x8000, 0xBFFF, 0xBFFF-0x8000, RWX_RW, mPrgRom[0].base);
        bus.mapRange(0xC000, 0xFFFF, 0xFFFF-0xC000, RWX_RW, mPrgRom[1].base);
    }

    // // PPU Mapping
    // {
    //     auto &bus = nes.mBusPPU;
    //     auto &ppu = nes.mPPU;
    //     ubyte *ppuRam = ppu.mVRAM.data();

    //     bus.mapRdWtRange(0x2000, 0x23FF, 1024-1, ppuRam + 0*1024);
    //     bus.mapRdWtRange(0x2400, 0x27FF, 1024-1, ppuRam + 1*1024);

    //     bus.mapRdWtRange(0x2800, 0x2BFF, 1024-1, ppuRam + 0*1024);
    //     bus.mapRdWtRange(0x2C00, 0x2FFF, 1024-1, ppuRam + 1*1024);
    // }

}



// static void MapCpuBus( System &nes )
// {
//     using namespace NesFile;

//     auto  &bus    = nes.mBusCPU;
//     auto  *gpak   = nes.mGPak;
//     auto  *fh     = gpak->mHeader;
//     ubyte *prgROM = gpak->mPrgROM.data();
//     ubyte *chrROM = gpak->mChrROM.data();

//     // CPU --> PRG ROM,  First 16KB of ROM
//     bus.mapRdRange(0x8000, 0xBFFF, 0xBFFF-0x8000, prgROM);

//     // CPU --> PRG ROM,  Mirror of 8000-BFFF (NROM-128) or final 16KB of ROM (NROM-256)
//     uint16_t off = (fh->prgRomSz == 2) ? 0x4000 : 0x0000;
//     bus.mapRdRange(0xC000, 0xFFFF, 0xFFFF-0xC000, prgROM+off);

// }

// static void MapPpuBus( System &nes )
// {
//     using namespace NesFile;

//     auto  &bus  = nes.mBusPPU;
//     auto  &ppu  = nes.mPPU;
//     auto  *gpak = nes.mGPak;

//     ubyte *chrROM = gpak->mChrROM.data();

//     // | Addr      | Size | Desc        | Mapped By |
//     // | ----------|------|-------------|-----------|
//     // | 0000-0FFF | 1000 | PtrnTable 0 | Cartridge |
//     // | 1000-1FFF | 1000 | PtrnTable 1 | Cartridge |
//     // | 2000-23BF | 03C0 | NameTable 0 | Cartridge |
//     // | 23C0-23FF | 0040 | AttrTable 0 | Cartridge |
//     // | 2400-27BF | 03c0 | Nametable 1 | Cartridge |
//     // | 27C0-27FF | 0040 | AttrTable 1 | Cartridge |
//     // | 2800-2BBF | 03c0 | Nametable 2 | Cartridge |
//     // | 2BC0-2BFF | 0040 | AttrTable 2 | Cartridge |
//     // | 2C00-2FBF | 03c0 | Nametable 3 | Cartridge |
//     // | 2FC0-2FFF | 0040 | AttrTable 3 | Cartridge |
//     // ----------------------------------------------

//     // PPU --> ChrROM
//     bus.mapRdRange(0x0000, 0x1FFF, 0xFFFF, chrROM);

//     // // PPU --> ChrRAM
//     // bus.mapRdWtRange(0x1000, 0x1FFF, 0x0FFF, chrRAM);

//     // PPU --> NameTables
//     ubyte *vRAM = ppu.mVRAM.data();

//     bus.mapRdWtRange(0x2000, 0x23FF, 1024-1, vRAM + 0*1024);
//     bus.mapRdWtRange(0x2400, 0x27FF, 1024-1, vRAM + 1*1024);

//     bus.mapRdWtRange(0x2800, 0x2BFF, 1024-1, vRAM + 0*1024);
//     bus.mapRdWtRange(0x2C00, 0x2FFF, 1024-1, vRAM + 1*1024);
// }

