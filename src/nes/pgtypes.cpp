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

ubyte PgEntryCpuPpu::read(addr_t addr)
{
    uint8_t idx = addr % 8;
    uint8_t data = 0;

    switch (idx)
    {
        case REG_PPUSTATUS:
            // ppu->STATUS.V = 1;
            data = (mPPU.STATUS.byte & 0xE0) | (mPPU.mData & 0x1F);
            mPPU.STATUS.V = 0;
            mPPU.mAddr.reset();
            break;

        case REG_OAMDATA:
        case REG_PPUDATA:
            data = mPPU.MMIO[idx];
            break;

        default:
            break;
    }

    return data;
}

void PgEntryCpuPpu::write(addr_t addr, ubyte data)
{
    uint8_t  idx = addr % 8;
    NesPPU  &ppu = mPPU;
    uint8_t *dst = ppu.MMIO + idx;

    switch (idx)
    {
        case REG_PPUCTRL:
        case REG_PPUMASK:
        case REG_OAMADDR:
        case REG_OAMDATA:
            *dst = data;
            break;

        case REG_PPUDATA:
            ppu.mBus.write(ppu.mAddr.value, data);
            ppu.mAddr.value += 1;
            break;

        case REG_PPUSCROLL:
            ppu.mScrl.write(data);
            break;

        case REG_PPUADDR:
            ppu.mAddr.write(data);
            break;

        default:
            break;
    }
}






ubyte PgEntryCpuIO::read(addr_t addr)
{
    auto &cpu = mCPU;
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


void PgEntryCpuIO::write(addr_t addr, ubyte data)
{
    auto &cpu = mCPU;

    if (addr == 0x4016)
    {
        if (data & 0x01)
        {
            cpu.mStdCtl0.hi();
            cpu.mStdCtl1.hi();
        }

        else
        {
            cpu.mStdCtl0.lo();
            cpu.mStdCtl1.lo(); 
        }
    }
}

