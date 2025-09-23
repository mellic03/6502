#include "pgtypes.hpp"


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


ubyte CpuPpuHandler::read(addr_t addr)
{
    uint8_t data = 0;

    switch (addr % 8)
    {
        case REG_PPUCTRL:
        case REG_PPUMASK:
            break;

        case REG_PPUSTATUS:
            data = ppu.read2002();
            break;

        case REG_OAMADDR:
        case REG_OAMDATA:
        case REG_PPUSCROLL:
        case REG_PPUADDR:
            break;

        case REG_PPUDATA:
            data = ppu.read2007();
            break;

        default:
            break;
    }

    return data;
}


void CpuPpuHandler::write(addr_t addr, ubyte data)
{
    switch (addr % 8)
    {
        case REG_PPUCTRL:
            printf("[WRITE PPUCTRL] data=%u\n", data);
            ppu.ppuctl = { data };
            break;
            
        case REG_PPUMASK:
            ppu.ppumask = { data };
            break;

        case REG_OAMADDR:
        case REG_OAMDATA:
            break;

        case REG_PPUSCROLL:
            ppu.write2005(data);
            break;

        case REG_PPUADDR:
            ppu.write2006(data);
            break;

        case REG_PPUDATA:
            ppu.write2007(data);
            break;

        default:
            break;
    }
}







ubyte CpuIoHandler::read(addr_t addr)
{
    ubyte data = 0;

    switch (addr)
    {
        case 0x4016:
            data = cpu.mStdCtl0.read();
            break;

        case 0x4017:
            data = cpu.mStdCtl1.read();
            break;

        default:
            break;
    }

    return data;
}


void CpuIoHandler::write(addr_t addr, ubyte data)
{
    if (addr == 0x4016)
    {
        // if (data & 0x01)
        // {
        //     cpu.mStdCtl0.hi();
        //     cpu.mStdCtl1.hi();
        // }

        // else
        // {
        //     cpu.mStdCtl0.lo();
        //     cpu.mStdCtl1.lo(); 
        // }
    }
}

