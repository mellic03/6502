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
        log("MirrorMode:    %u", fh->MirroringMode);
        log("4ScreenVRAM:   %u", fh->FourScreenVRAM);
        log("PAL/NTSC:      %s", (fh->IsPAL) ? "PAL" : "NTSC");
    }

    mPrgRam = {new ubyte[4*1024], 4*1024};

    mPrgRom = {fpos, fh->PrgRomNo16K * 16*1024};
    fpos += mPrgRom.size;

    mChrRom = {fpos, fh->ChrRomNo8K * 8*1024};
    fpos += mChrRom.size;

    // CPU Mapping
    {
        nes.mBusCPU.mapRange(0x0000, 0xFFFF, new CpuAccess(nes, *this));
        // auto &bus = nes.mBusCPU;

        // // CPU --> PRG RAM
        // bus.mapRWRange(0x6000, 0x7FFF, mPrgRam.base, mPrgRam.size);

        // // CPU --> PRG ROM
        // bus.mapRdRange(0x8000, 0xFFFF, mPrgRom.base, mPrgRom.size);
    }

    // PPU Mapping
    {
        nes.mBusPPU.mapRange(0x0000, 0x3FFF, new PpuAccess(nes, *this));
        nes.mPPU.mMirrorMode = (fh->MirroringMode == 0) ? 0 : 1;

        // auto &bus = nes.mBusPPU;

        // // PPU --> CHR ROM
        // bus.mapRdRange(0x0000, 0x1FFF, mChrRom.base, mChrRom.size);
    }

}

ubyte Mapper000_NROM::CpuAccess::read(addr_t addr)
{
    if (0x0000<=addr && addr<=0x1FFF)
    {
        ubyte *cpuram = cpu.mRAM.data();
        return cpuram[addr % 2048];
    }

    if (0x2000<=addr && addr<=0x3FFF)
    {
        return read_ppu(addr);
    }

    if (0x4000<=addr && addr<=0x4015)
    {
        return 0;
    }

    if (0x4016<=addr && addr<=0x4017)
    {
        ubyte *mmio = cpu.mMMIO;
        ubyte data = (mmio[addr-0x4000] & 0b01);
        mmio[addr-0x4000] >>= 1;
        return data;
    }

    // if (0x4018<=addr && addr<=0x5FFF)
    // {
    //     return 0;
    // }

    if (0x6000<=addr && addr<=0x7FFF)
    {
        ubyte *ram  = nrom.mPrgRam.base;
        size_t size = nrom.mPrgRam.size;
        return ram[(addr - 0x6000) % size];
    }

    if (0x8000<=addr && addr<=0xFFFF)
    {
        ubyte *prgrom = nrom.mPrgRom.base;
        return prgrom[(addr - 0x8000) % nrom.mPrgRom.size];
    }

    printf("[CpuAccess::read] addr=0x%04X\n", addr);
    assert(false);
    return 0;
}


void Mapper000_NROM::CpuAccess::write(addr_t addr, ubyte data)
{
    if (0x0000<=addr && addr<=0x1FFF)
    {
        ubyte *cpuram = cpu.mRAM.data();
        cpuram[addr % 2048] = data;
    }

    if (0x2000<=addr && addr<=0x3FFF)
    {
        write_ppu(addr, data);
    }

    if (0x4014<=addr && addr<=0x4014)
    {
        write_ppu(addr, data);
    }

    if (0x4016<=addr && addr<=0x4016) // controller strobe
    {
        ubyte *mmio = cpu.mMMIO;
        ubyte *gpad = (ubyte*)(nes.mPlayerCtl);
        ubyte  prev = (mmio[0x16] & 0b01);
        ubyte  curr = (data & 0b01);
        mmio[0x16] = curr;

        // if released, load state of all 8 buttons into shift registers
        if (prev==1 && curr==0)
        {
            mmio[0x16] = gpad[0];
            mmio[0x17] = gpad[1];
        }

        else
        {
            mmio[0x16] = curr;
        }
    }

    if (0x6000<=addr && addr<=0x7FFF)
    {
        ubyte *ram  = nrom.mPrgRam.base;
        size_t size = nrom.mPrgRam.size;
        ram[(addr - 0x6000) % size] = data;
    }
}



ubyte Mapper000_NROM::CpuAccess::read_ppu(addr_t addr)
{
    // printf("[read_ppu] 0x%04X\n", addr);
    nes.cycleAccumFlush();

    auto  &ppuctl  = ppu.ppuctl;
    auto  &ppumask = ppu.ppumask;
    auto  &ppustat = ppu.ppustat;
    auto  &oamaddr = ppu.oamaddr;
    ubyte &oamdata = ppu.oamdata;
    uword ppuaddr = ppu.ppuaddr;
    ubyte ppudata = ppu.ppudata;
    uint8_t data = 0;

    switch (0x2000 + (addr % 8))
    {
        case 0x2000: // REG_PPUCTRL
        case 0x2001: // REG_PPUMASK
            break;

        case 0x2002: // REG_PPUSTATUS
            // printf("REG_PPUSTATUS read\n");
            data = ppustat.byte;
            ppustat.VBlank = 0;
            mAddrLatch = true;
            break;

        case 0x2003: // REG_OAMADDR
            break;

        case 0x2004: // REG_OAMDATA
            oamdata = ppu.rdbus(oamaddr);
            data = oamdata;
            break;

        case 0x2005: // REG_PPUSCROLL
            break;

        case 0x2006: // REG_PPUADDR
            break;

        case 0x2007: // REG_PPUDATA
            // printf("REG_PPUDATA read\n");
            data = ppudata;
            ppudata = ppu.rdbus(ppuaddr);
            if (ppuaddr >= 0x3F00)
                data = ppudata;
            ppuaddr += (ppuctl.Increment) ? 32 : 1;
            ppuaddr &= 0x3FFF;
            break;

        default:
            break;
    }

    return data;
}


void Mapper000_NROM::CpuAccess::write_ppu(addr_t addr, ubyte data)
{
    // printf("[write_ppu] 0x%04X\n", addr);
    nes.cycleAccumFlush();

    auto &ppuctl  = ppu.ppuctl;
    auto &ppumask = ppu.ppumask;
    auto &ppustat = ppu.ppustat;
    auto &oamaddr = ppu.oamaddr;
    auto &oamdata = ppu.oamdata;
    auto &ppuaddr = ppu.ppuaddr;
    auto &ppudata = ppu.ppudata;

    if (addr == 0x4014)
    {
        uword base = 256*uword(data);

        for (uword i=0; i<256; i++)
        {
            // printf("OAM[0x%02X] == RAM[0x%04X] == 0x%02X\n", oamaddr, base+i, cpu.rdbus(base+i));
            ppu.mOAM[oamaddr++] = cpu.rdbus(base+i);
        }
        return;
    }

    switch (0x2000 + (addr % 8))
    {
        case 0x2000: // REG_PPUCTRL
            ppuctl = { data };
            break;
            
        case 0x2001: // REG_PPUMASK
            ppumask = { data };
            break;

        case 0x2002: // REG_PPUSTATUS
            break;

        case 0x2003: // REG_OAMADDR
            oamaddr = data;
            break;

        case 0x2004: // REG_OAMDATA
            ppu.mOAM[oamaddr++] = data;
            break;

        case 0x2005: // REG_PPUSCROLL
            break;

        case 0x2006: // REG_PPUADDR
            if ( mAddrLatch) ppu.ppuaddr_hi = data;
            if (!mAddrLatch) ppu.ppuaddr_lo = data;
            mAddrLatch = !mAddrLatch;
            break;

        case 0x2007: // REG_PPUDATA
            ppu.wtbus(ppuaddr, data);
            ppuaddr += (ppuctl.Increment) ? 32 : 1;
            ppuaddr &= 0x3FFF;
            break;

        default:
            break;
    }
}




ubyte Mapper000_NROM::PpuAccess::read(addr_t addr)
{
    if (0x0000<=addr && addr<=0x1FFF)
    {
        ubyte *rom  = nrom.mChrRom.base;
        size_t size = nrom.mChrRom.size;
        return rom[addr % size];
    }

    if (0x2000<=addr && addr<=0x3EFF)
    {
        ubyte *vram = ppu.mVRAM;
        return vram[(addr - 0x2000) % 2048];
    }

    if (0x3F00<=addr && addr<=0x3FFF)
    {
        addr &= 0x1F;
        addr &= ~0x10;
        if (addr == 0x10) addr = 0x00;
        if (addr == 0x14) addr = 0x04;
        if (addr == 0x18) addr = 0x08;
        if (addr == 0x1C) addr = 0x0C;
        return ppu.mPaletteCtl[addr];
    }

    printf("[PpuAccess::read] addr=0x%04X\n", addr);
    assert(false);
    return 0;
}


void Mapper000_NROM::PpuAccess::write(addr_t addr, ubyte data)
{
    if (0x2000<=addr && addr<=0x3EFF)
    {
        ppu.mVRAM[(addr - 0x2000) % 2048] = data;
    }

    if (0x3F00<=addr && addr<=0x3FFF)
    {
        addr &= 0x1F;
        if (addr == 0x10) addr = 0x00;
        if (addr == 0x14) addr = 0x04;
        if (addr == 0x18) addr = 0x08;
        if (addr == 0x1C) addr = 0x0C;
        ppu.mPaletteCtl[addr & 0x1F] = data;
    }
}
