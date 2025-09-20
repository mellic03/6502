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

static uint8_t CpuRdPpu(memu::HwModule*, addr_t);
static void CpuWtPpu(memu::HwModule*, addr_t, ubyte);



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
        // log::Info("PAL/NTSC: %s\n", (fh->IsPAL) ? "PAL" : "NTSC");
    }

    mPrgRom = {fpos, fh->PrgRomNo16K * 16*1024};
    fpos += mPrgRom.size;

    mChrRom = {fpos, fh->ChrRomNo8K * 8*1024};
    fpos += mChrRom.size;

    auto &ppu = nes.mPPU;
    auto &cpu = nes.mCPU;
    ubyte *cpuRAM = cpu.mRAM.data();
    ubyte *ppuRAM = ppu.mVRAM.data();

    // CPU Mapping
    {
        auto &bus = nes.mBusCPU;

        // CPU --> CPU RAM.
        bus.mapRange(0x0000, 0x0800-1, 2047, memu::RWX_RW, cpuRAM);
        bus.mapRange(0x0800, 0x1000-1, 2047, memu::RWX_RW, cpuRAM);
        bus.mapRange(0x1000, 0x1800-1, 2047, memu::RWX_RW, cpuRAM);
        bus.mapRange(0x1800, 0x2000-1, 2047, memu::RWX_RW, cpuRAM);

        // CPU --> PPU MMIO registers.
        bus.mapRangeTiny(0x2000, 0x3FFF, &ppu, CpuRdPpu, CpuWtPpu);

        // CPU --> APU and IO registers. 4000 - 401F
        bus.mapRange(0x4000, 0x40FF, 0x00FF, memu::RWX_RW, &(cpu.mApuMMIO));

        // CPU --> PGR ROM
        bus.mapRdRange(0x8000, 0xBFFF, mPrgRom.base);

        uint8_t sz = fh->PrgRomNo16K;
        if (sz==2) bus.mapRdRange(0xC000, 0xFFFF, mPrgRom.base + 16*1024);
        else       bus.mapRdRange(0xC000, 0xFFFF, mPrgRom.base);
    }

    // PPU Mapping
    {
        auto &bus = nes.mBusPPU;

        // PPU --> CHR ROM
        bus.mapRange(0x0000, 0x1FFF, 0x0FFF, memu::RWX_RW, mChrRom.base);

        // PPU --> PPU Nametables
        bus.mapRange(0x2000, 0x2FFF, 0x0FFF, memu::RWX_RW, ppuRAM);
        bus.mapRange(0x3000, 0x3EFF, 0x0EFF, memu::RWX_RW, ppuRAM);

        // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 0x3FFF
        bus.mapRange(0x3F00, 0x3FFF, 0x001F, memu::RWX_RW, ppu.mPalette);
    }

}






enum REG_: uint16_t
{
    REG_PPUCTRL = 0x0000,
    REG_PPUMASK,
    REG_PPUSTATUS,
    REG_OAMADDR,
    REG_OAMDATA,
    REG_PPUSCROLL,
    REG_PPUADDR,
    REG_PPUDATA,
    REG_OAMDMA = 0x4014,
};


static uint8_t CpuRdPpu( memu::HwModule *dev, addr_t addr )
{
    uint8_t idx = addr % 8;
    NesPPU *ppu = (NesPPU*)dev;

    switch (idx)
    {
        default: break;

        case REG_PPUSTATUS:
        case REG_OAMDATA:
        case REG_PPUDATA:
            return ppu->MMIO[idx];
    }

    return 0;
}


static void CpuWtPpu( memu::HwModule *dev, addr_t addr, ubyte data )
{
    uint8_t  idx = addr % 8;
    NesPPU  *ppu = (NesPPU*)dev;
    uint8_t *dst = ppu->MMIO + idx;

    switch (idx)
    {
        default: break;

        case REG_PPUCTRL:
        case REG_PPUMASK:
        case REG_OAMADDR:
        case REG_OAMDATA:
        case REG_PPUDATA:
            *dst = data;
            break;

        case REG_PPUSCROLL:
            ppu->mScrl.write(data);
            break;

        case REG_PPUADDR:
            ppu->mAddr.write(data);
            break;
    }
}

