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
memu::ConfigParser NesEmu::CONF("./nes.conf");


NesEmu::System::System()
:   mCPU(mBusCPU),
    mPPU(mBusPPU),
    mGPak(nullptr)
{
    using namespace memu;

    // CPU Mapping
    // -------------------------------------------------------------------------
    ubyte *cpuram = mCPU.mRAM.data();
    size_t cpursz = mCPU.mRAM.size();

    // CPU --> CPU RAM.
    mBusCPU.mapRWRange(0x0000, 0x1FFF, cpuram, cpursz);

    // // CPU --> PPU MMIO registers.
    mBusCPU.mapRange(0x2000, 0x3FFF, new CpuPpuHandler(mPPU));

    // CPU --> APU and IO registers. 4000 - 401F
    mBusCPU.mapRange(0x4000, 0x40FF, new CpuIoHandler(mCPU));
    // -------------------------------------------------------------------------


    // PPU Mapping
    // -------------------------------------------------------------------------
    // uint8_t *ppuram = mPPU.mVRAM.data();
    // size_t   ppursz = mPPU.mVRAM.size();

    // // PPU --> PPU VRAM
    // mBusPPU.mapRWRange(0x2000, 0x2FFF, ppuram, ppursz);
    // mBusPPU.mapRWRange(0x3000, 0x3EFF, ppuram, ppursz);

    // // PPU --> PPU Pallete Indices. 3F00 - 3F1F. Mirrored to 3FFF
    // mBusPPU.mapRWRange(0x3F00, 0x3FFF, mPPU.mPaletteCtl, sizeof(mPPU.mPaletteCtl));
    // -------------------------------------------------------------------------
}



void NesEmu::System::loadGamePak( GamePak *gpak )
{
    mGPak = gpak;
    Mapper::MapGamePak(*this, gpak);

    if (CONF["boot"]["jump"])
    {
        (uint16_t)strtol(CONF["boot"]["jump"], NULL, 16);
    }

    else
    {
        mCPU.PC = (mBusCPU.read(0xFFFD) << 8) | mBusCPU.read(0xFFFC);
    }
}


void NesEmu::System::tick()
{
    static int curr  = 0;
    static int prev  = 0;
    static int accum = 0;

    mBusPPU.tick();
    mBusPPU.tick();
    mBusPPU.tick();

    if (mPPU.STATUS.V == 1)
        mBusCPU.line_nmi = 1;
    else
        mBusCPU.line_nmi = 0;

    mCPU.mScanLine = mPPU.mScanLine;
    mCPU.mScanDot  = mPPU.mScanDot;
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


