#include <memu/nes/ppu.hpp>
#include <memu/nes/nes.hpp>
#include <memu/addrspace.hpp>
#include <memu/display.hpp>
#include <memu/pinout.hpp>


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


ubyte NesPPU::CpuAccess::read(addr_t addr)
{
    uint8_t data = 0;

    auto &ppuctl  = ppu.ppuctl;
    auto &ppumask = ppu.ppumask;
    auto &ppustat = ppu.ppustat;
    auto &oamaddr = ppu.oamaddr;
    auto &oamdata = ppu.oamdata;
    auto &ppuaddr = ppu.ppuaddr;
    auto &ppudata = ppu.ppudata;

    switch (0x2000 + (addr % 8))
    {
        case 0x2000: // REG_PPUCTRL
        case 0x2001: // REG_PPUMASK
            break;

        case 0x2002: // REG_PPUSTATUS
            data = ppustat.byte;
            ppustat.VBlank = 0;
            mAddrLatch = true;
            break;

        case 0x2003: // REG_OAMADDR
        case 0x2004: // REG_OAMDATA
        case 0x2005: // REG_PPUSCROLL
        case 0x2006: // REG_PPUADDR
            break;

        case 0x2007: // REG_PPUDATA
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


void NesPPU::CpuAccess::write(addr_t addr, ubyte data)
{
    auto &ppuctl  = ppu.ppuctl;
    auto &ppumask = ppu.ppumask;
    auto &ppustat = ppu.ppustat;
    auto &oamaddr = ppu.oamaddr;
    auto &oamdata = ppu.oamdata;
    auto &ppuaddr = ppu.ppuaddr;
    auto &ppudata = ppu.ppudata;

    switch (0x2000 + (addr % 8))
    {
        case 0x2000: // REG_PPUCTRL
            ppu.ppuctl = { data };
            break;
            
        case 0x2001: // REG_PPUMASK
            ppu.ppumask = { data };
            break;

        case 0x2002: // REG_PPUSTATUS
            break;

        case 0x2003: // REG_OAMADDR
            break;

        case 0x2004: // REG_OAMDATA
            break;

        case 0x2005: // REG_PPUSCROLL
            break;

        case 0x2006: // REG_PPUADDR
            if (mAddrLatch) { ppuaddr = (ppuaddr & 0x00FF) | (uword(data) << 8); }
            else            { ppuaddr = (ppuaddr & 0xFF00) | (uword(data) << 0); }
            mAddrLatch = !mAddrLatch;

        case 0x2007: // REG_PPUDATA
            ppu.wtbus(ppuaddr, data);
            ppuaddr += (ppuctl.Increment) ? 32 : 1;
            ppuaddr &= 0x3FFF;
            break;

        default:
            break;
    }
}

