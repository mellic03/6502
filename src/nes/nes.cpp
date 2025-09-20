#include <memu/nes/nes.hpp>
#include <memu/nes/mapper.hpp>
#include <stdio.h>
#include <string.h>


/*
    https://www.nesdev.org/wiki/CPU_memory_map

    | Address range | Size | Device                                                 |
    |-------------------------------------------------------------------------------|
    | 0000 – 07FF   | 0800 | 2 KB internal RAM                                      |
    |-------------------------------------------------------------------------------|
    | 0800 – 0FFF   | 0800 |                                                        |
    | 1000 – 17FF   | 0800 | Mirrors of 0000 – 07FF                                 |
    | 1800 – 1FFF   | 0800 |                                                        |
    |-------------------------------------------------------------------------------|
    | 2000 – 2007   | 0008 | NES PPU registers                                      |
    |-------------------------------------------------------------------------------|
    | 2008 – 3FFF   | 1FF8 | Mirrors of 2000 – 2007 (repeats every 8 bytes)         |
    |-------------------------------------------------------------------------------|
    | 4000 – 4017   | 0018 | NES APU and I/O registers                              |
    |-------------------------------------------------------------------------------|
    | 4018 – 401F   | 0008 | APU and I/O functionality that is normally disabled.   |
    |-------------------------------------------------------------------------------|
    | 4020 – FFFF   | BFE0 | Unmapped. Available for cartridge use.                 |
    | 6000 - 7FFF   | 2000 | Usually cartridge RAM, when present.                   |
    | 8000 - FFFF   | 8000 | Usually cartridge ROM and mapper registers.            |
    |-------------------------------------------------------------------------------|


*/


void NesEmu::System::loadGamePak( GamePak *gpak )
{
    mGPak = gpak;
    Mapper::MapGamePak(*this, gpak);
    mCPU.PC = (mBusCPU.read(0xFFFD) << 8) | mBusCPU.read(0xFFFC);
}


void NesEmu::System::tick()
{
    static int curr  = 0;
    static int prev  = 0;
    static int accum = 0;

    mBusPPU.tick();
    mBusPPU.tick();
    mBusPPU.tick();
    mBusCPU.tick();

    // mBusCPU.tick();

    // curr = mCPU.clockTime();
    // accum += curr - prev;
    // prev = curr;

    // if (accum >= 3)
    // {
    //     while (accum > 0)
    //     {
    //         mBusPPU.tick();
    //         accum -= 1;
    //     }
    // }

    accum = std::max(accum, 0);
}








static uint8_t CpuRdPpu(memu::HwModule*, addr_t);
static void CpuWtPpu(memu::HwModule*, addr_t, ubyte);

static uint8_t CpuRdIO(memu::HwModule*, addr_t);
static void CpuWtIO(memu::HwModule*, addr_t, ubyte);


NesEmu::System::System()
:   mCPU(mBusCPU), mPPU(mBusPPU),
    mGPak(nullptr)
{
    using namespace memu;

    // CPU Mapping
    // -------------------------------------------------------------------------
    ubyte *cpuram = mCPU.mRAM.data();
    size_t cpursz = mCPU.mRAM.size();

    // CPU --> CPU RAM.
    mBusCPU.mapRange(0x0000, 0x1FFF, cpursz-1, RWX_RW, cpuram);

    // CPU --> PPU MMIO registers.
    mBusCPU.mapRangeTiny(0x2000, 0x3FFF, &mPPU, CpuRdPpu, CpuWtPpu);

    // CPU --> APU and IO registers. 4000 - 401F
    mBusCPU.mapRangeTiny(0x4000, 0x40FF, &mCPU, CpuRdIO, CpuWtIO);
    // -------------------------------------------------------------------------


    // PPU Mapping
    // -------------------------------------------------------------------------
    uint8_t *ppuram = mPPU.mVRAM.data();
    size_t   ppursz = mPPU.mVRAM.size();

    // PPU --> PPU Nametables
    mBusPPU.mapRange(0x2000, 0x2FFF, ppursz-1, RWX_RW, ppuram);
    mBusPPU.mapRange(0x3000, 0x3EFF, ppursz-1, RWX_RW, ppuram);

    // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 3FFF
    mBusPPU.mapRange(0x3F00, 0x3FFF, 32-1, RWX_RW, mPPU.mPaletteIndices);
    // bus.mapRange(0x3F00, 0x3FFF, 0x001F, RWX_RW, mPPU.mPalette);
    // -------------------------------------------------------------------------

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
    uint8_t data = 0;

    switch (idx)
    {
        case REG_PPUSTATUS:
            // ppu->STATUS.V = 1;
            data = (ppu->STATUS.byte & 0xE0) | (ppu->mData & 0x1F);
            ppu->STATUS.V = 0;
            ppu->mAddr.reset();
            break;

        case REG_OAMDATA:
        case REG_PPUDATA:
            data = ppu->MMIO[idx];
            break;

        default:
            break;
    }

    return data;
}


static void CpuWtPpu( memu::HwModule *dev, addr_t addr, ubyte data )
{
    uint8_t  idx = addr % 8;
    NesPPU  &ppu = *(NesPPU*)dev;
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



static uint8_t CpuRdIO( memu::HwModule *dev, addr_t addr )
{
    auto  &cpu = *(NesCPU*)dev;
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


static void CpuWtIO( memu::HwModule *dev, addr_t addr, ubyte data )
{
    auto &cpu = *(NesCPU*)dev;

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


