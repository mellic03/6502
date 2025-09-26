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

NesEmu::System::System()
:   mConf("nes.conf"),
    mEmuIO(),
    mGameWin(mEmuIO.makeWin("NesEmu", 256, 240, 4, 1)),
    mChrWin(mEmuIO.makeWin("CHR-ROM", 128, 256, 4, 1)),
    // mChrWin(new EmuFramebuffer(128, 128)),
    mCPU(mBusCPU),
    mPPU(mBusPPU, mGameWin, mChrWin)
{
    using namespace memu;
    memset(mPlayerCtl, 0, sizeof(mPlayerCtl));

    // pinout mappings
    // -------------------------------------------------------------------------
    ioCLK = 0;
    ioIRQ = 0;
    ioNMI = 0;
    ioRES = 0;

    mCPU.ioLineCLK = &ioCLK;
    mCPU.ioLineIRQ = &ioIRQ;
    mCPU.ioLineNMI = &ioNMI;
    mCPU.ioLineRES = &ioRES;

    mPPU.ioCLK = &ioCLK;
    mPPU.ioINT = &ioNMI;
    mPPU.ioRES = &ioRES;
    // -------------------------------------------------------------------------

}



void NesEmu::System::loadGamePak( GamePak *gpak )
{
    Mapper::MapGamePak(*this, gpak);

    mPPU.loadPalette(mConf["video"]["palette"]);
    mPPU.preRenderChrRom(mChrWin);
    mChrWin->flush();

    mCPU.reset();
    // mCPU.PC = mCPU.rdbusw(0xFFFC);

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

    while ((3*accum) - 3 >= 0)
    {
        clocks = mPPU.clockTime();
        mPPU.tick();
        accum -= (mPPU.clockTime() - clocks);
    }

    // int rem = mPPU.tick(mCPU.clockTime() - start);
    // if (rem)
    // {
    //     mPPU.tick(rem);
    // }

    // mCPU.tick();
    // mPPU.tick();
    // // ioRES = 1;
    // mPPU.tick();
    // mPPU.tick();


    // if (mPPU.mFrameDone == true)
    // {
    //     mWin->flush();
    //     mPPU.mFrameDone = false;
    // }

    // static size_t accum = 0;
    // static size_t prev = 0;

    // accum += mPPU.clockTime() - prev;
    // prev   = mPPU.clockTime();

    // while (accum > 3)
    // {
    //     mBusCPU.tick();
        
    // }

    // {
    //     mPPU.tick(mWin);
    //     mBusPPU.tick();
    //     mBusPPU.tick();
    //     mBusPPU.tick();
    // }

    mClocks += 1;
}


