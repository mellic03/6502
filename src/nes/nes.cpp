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
    // mChrWin(io->makeWin("CHR-ROM", 128, 256, 4, 0)),
    mCPU(mBusCPU),
    mPPU(mBusPPU, mGameWin),
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

    loadGamePak(new NesEmu::GamePak(mConf["boot"]["rom"]));

    mPPU.flush();
    mPPU.flush();
    mPPU.flush();
}



void NesEmu::System::loadGamePak( GamePak *gpak )
{
    Mapper::MapGamePak(*this, gpak);

    mPPU.loadPalette(mConf["video"]["palette"]);
    // mPPU.preRenderChrRom(mChrWin);
    // mChrWin->flush();
    mCPU.reset();

    if (mConf["boot"]["jump"])
    {
        mCPU.PC = (uint16_t)strtol(mConf["boot"]["jump"], NULL, 16);
    }

    printf("PC: %04X\n", mCPU.PC);
}



void NesEmu::System::tick()
{
    if (ioLineRES)
    {
        mCPU.reset();
        mPPU.reset();
        ioLineRES = 0;
        mCycleAccum = 0;
    }

    int clocks = mCPU.clockTime();
    for (int i=0; i<4; i++)
    {
        mCPU.tick();
    }
    mCycleAccum += 3 * (mCPU.clockTime() - clocks);

    if (mCPU.mWaiting || mCycleAccum >= 64)
    {
        cycleAccumFlush();
    }

}







#include <fstream>
#include <sstream>


void print_row( NesTest::Row &row )
{
    // pc, op, ac, xr, yr, ssr, sp, ppuLine, ppuDot, cycle;
    // printf("C000 4C 00 00 00 24 FD 0000 0033 0007 \n");
    
    printf("%04X ", row.col[0]);
    for (int i=1; i<5; i++)
        printf("%02X ", row.col[i]);

    for (int i=7; i>=0; i--)
        printf("%u", bool(row.col[5] & (1<<i)));
    printf(" ");

    printf("%02X ", row.sp);

    // for (int i=7; i<10; i++)
    //     printf("%04u ", row.col[i]);
    // printf("\n");
}



void NesTest::compare( const std::string &path, NesCPU &cpu )
{
    NesTest test0(path);

    cpu.PC = 0xC000;

    for (size_t i=0; i<test0.size(); i++)
    {
        using namespace memu;

        uword pc = cpu.PC;
        ubyte op = cpu.rdbus(cpu.PC);
        Row row = {pc, op, cpu.AC, cpu.XR, cpu.YR, cpu.SSR.byte, cpu.SP, 0, 0, cpu.clockTime()};

        cpu._fetch();
        cpu._decode();
        cpu._execute();

        if (cpu.mInvalidOp)
        {
            printf("Invalid opcode (0x%02X)\n", cpu.mCurrOp);
            break;
        }


        printf("%04d   ADDR OP AC XR YR NVBUDIZC SP \n", i+1);
        printf("ideal  ");
        print_row(test0[i]);
        printf("\n");

        printf("actual ");
        print_row(row);
        printf("[ ");
        ubyte sp = row.sp;
        for (int i=0; i<10&&sp+i<0xFF; i++)
            printf("%02X ", cpu.rdbus(0x0100 + sp+i));
        printf("]\n");

        if (test0[i] != row)
        {
            printf("Fail\n");
            break;
        }
        printf("\n");
    }
}


NesTest::NesTest( const std::string &path )
{
    std::ifstream stream(path);
    std::string line = "";

    while (std::getline(stream, line, '\n'))
    {
        std::stringstream ss(line);
        std::string word = "";
        Row row;
        int col = 0;

        while (std::getline(ss, word, ' '))
        {
            if (word.length()==0 || word[0]==' ')
            {
                continue;
            }

            row.col[col++] = (uint64_t)strtol(word.c_str(), NULL, 16);

            if (col == 10)
            {
                mData.push_back(row);
                col = 0;
                // printf("%04X ", row.col[0]);
                // for (int i=1; i<7; i++)
                //     printf("%02X ", row.col[i]);
                // for (int i=7; i<10; i++)
                //     printf("%04u ", row.col[i]);
                // printf("\n");
            }
        }
    }
}


// #include <memu/pinout.hpp>

// NesTest::NesTest( NesCPU &cpu )
// {
//     cpu.PC = 0xC000;
//     int count = 0;

//     while (!cpu.mInvalidOp && count++ < 512)
//     {
//         using namespace memu;

//         uword pc = cpu.PC;
//         ubyte op = cpu.rdbus(cpu.PC);
//         Row row = {pc, op, cpu.AC, cpu.XR, cpu.YR, cpu.SSR.byte, cpu.SP, 0, 0, cpu.clockTime()};

//         cpu._fetch();
//         cpu._decode();
//         cpu._execute();

//         if (cpu.mInvalidOp)
//         {
//             printf("Invalid opcode (0x%02X)\n", cpu.mCurrOp);
//         }

//         mData.push_back(row);
//     }
// }

