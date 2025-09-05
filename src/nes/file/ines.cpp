#include "ines.hpp"
#include <cstring>
#include <cstdio>
using namespace NesFile;


iNES::FileInfo::FileInfo( uint8_t *data, size_t size )
{
    auto &H  = *(iNES::FileHeader*)data;
    auto &f6 = H.f6;
    auto &f7 = H.f7;

    mapperNo = (uint8_t(f7.mapNo_hi4) << 4) | f6.mapNo_lo4;

    headerSz  = 16;
    trainerSz = (f6.trainer512byte) ? 512 : 0;
    prgRomSz  = 16*1024 * H.prgNo16kBanks;
    chrRomSz  =  8*1024 * H.chrNo8kBanks;
    playChInstRomSz = 0;
    playChPRomSz = 0;
    titleSz = 0;

    headerOff        = 0x0000;
    trainerOff       = headerOff + headerSz;
    prgRomOff        = trainerOff + trainerSz;
    chrRomOff        = prgRomOff + prgRomSz;
    playChInstRomOff = chrRomOff + chrRomSz;
    playChPRomOff    = playChInstRomOff + playChInstRomSz;
    titleOff         = playChPRomOff + playChPRomSz;
    titleSz          = size - titleOff;


    printf("iNES::mInfo\n");
    printf("----------------------\n");
    printf("header        0x%04X --> 0x%04X\n", headerOff, headerOff+headerSz);
    printf("trainer       0x%04X --> 0x%04X\n", trainerOff, trainerOff+trainerSz);
    printf("prgRom        0x%04X --> 0x%04X\n", prgRomOff, prgRomOff+prgRomSz);
    printf("chrRom        0x%04X --> 0x%04X\n", chrRomOff, chrRomOff+chrRomSz);
    printf("playChInstRom 0x%04X --> 0x%04X\n", playChInstRomOff, playChInstRomOff+playChInstRomSz);
    printf("playChPRom    0x%04X --> 0x%04X\n", playChPRomOff, playChPRomOff+playChPRomSz);
    printf("title         0x%04X --> 0x%04X\n", titleOff, titleOff+titleSz);
    printf("----------------------\n\n");
}



NesFile::iNES::iNES( uint8_t *raw, size_t size )
:   NesFile::Base(raw, size),
    mInfo(raw, size)
{
    mPrgROM.reset(mInfo.prgRomSz);
    mChrROM.reset(mInfo.chrRomSz);

}

