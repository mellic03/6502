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

NesEmu::System::System( EmuIO *io )
:   mConf("nes.conf"),
    mGameWin(io->makeWin("NesEmu", 256, 240, 4, 0)),
    mChrWin(io->makeWin("CHR-ROM", 256, 128, 4, 0)),
    mCPU(mBusCPU),
    mPPU(mBusPPU, mGameWin, mChrWin),
    ioLineNMI(0), ioLineRES(0), ioLineIRQ(0), ioLineCLK(0)
{
    using namespace memu;
    memset(mPlayerCtl, 0, sizeof(mPlayerCtl));

    // pinout mappings
    // -------------------------------------------------------------------------
    mCPU.ioLineCLK = &ioLineCLK;
    mCPU.ioLineIRQ = &ioLineIRQ;
    mCPU.ioLineNMI = &ioLineNMI;
    mCPU.ioLineRES = &ioLineRES;

    mPPU.ioLineCLK = &ioLineCLK;
    mPPU.ioLineNMI = &ioLineNMI;
    mPPU.ioLineRES = &ioLineRES;
    // -------------------------------------------------------------------------
}



void NesEmu::System::loadGamePak( GamePak *gpak )
{
    Mapper::MapGamePak(*this, gpak);

    mPPU.loadPalette(mConf["video"]["palette"]);
    mPPU.preRenderChrRom(mChrWin);
    mChrWin->flush();
    mCPU.reset();

    if (mConf["boot"]["jump"])
    {
        mCPU.PC = (uint16_t)strtol(mConf["boot"]["jump"], NULL, 16);
    }

    printf("PC: %04X\n", mCPU.PC);
}


void NesEmu::System::tick()
{
    static int accum = 0;
    int clocks = 0;

    clocks = mCPU.clockTime();
    mCPU.tick();
    accum += (mCPU.clockTime() - clocks);

    while (3*accum - 3 >= 0)
    {
        clocks = mPPU.clockTime();
        mPPU.tick();
        accum -= (mPPU.clockTime() - clocks);
    }

    // // int rem = mPPU.tick(mCPU.clockTime() - start);
    // // if (rem)
    // // {
    // //     mPPU.tick(rem);
    // // }

    // mCPU.tick();
    // mPPU.tick();
    // mPPU.tick();
    // mPPU.tick();

}


