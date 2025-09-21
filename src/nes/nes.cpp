#include <memu/nes/nes.hpp>
#include <memu/nes/mapper.hpp>
#include "pgtypes.hpp"
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








// static uint8_t CpuRdPpu(memu::HwModule*, addr_t);
// static void CpuWtPpu(memu::HwModule*, addr_t, ubyte);

// static uint8_t CpuRdIO(memu::HwModule*, addr_t);
// static void CpuWtIO(memu::HwModule*, addr_t, ubyte);


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
    mBusCPU.mapRange(0x0000, 0x1FFF, RWX_RW, cpuram, cpursz);

    // // CPU --> PPU MMIO registers.
    mBusCPU.mapiRange<PgEntryCpuPpu>(0x2000, 0x3FFF, mPPU);

    // CPU --> APU and IO registers. 4000 - 401F
    mBusCPU.mapiRange<PgEntryCpuIO>(0x4000, 0x401F, mCPU);
    // -------------------------------------------------------------------------


    // PPU Mapping
    // -------------------------------------------------------------------------
    uint8_t *ppuram = mPPU.mVRAM.data();
    size_t   ppursz = mPPU.mVRAM.size();

    // PPU --> PPU VRAM
    mBusPPU.mapRange(0x2000, 0x2FFF, RWX_RW, ppuram, ppursz);
    mBusPPU.mapRange(0x3000, 0x3EFF, RWX_RW, ppuram, ppursz);

    // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 3FFF
    mBusPPU.mapRange(0x3F00, 0x3FFF, RWX_RW, mPPU.mPaletteCtl, sizeof(mPPU.mPaletteCtl));
    // mBusPPU.mapRange(0x3F00, 0x3FFF, RWX_RW, mPPU.mPalette, sizeof(mPPU.mPalette));
    // -------------------------------------------------------------------------

}

