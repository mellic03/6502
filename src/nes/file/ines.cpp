#include "ines.hpp"
#include <cstring>
#include <cstdio>
using namespace NesFile;


iNES::FileInfo::FileInfo( uint8_t *data, size_t size )
{
    auto &H  = *(iNES::FileHead*)data;
    auto &f6 = H.f6;
    auto &f7 = H.f7;
    auto &f8 = H.f8;
    auto &f9 = H.f9;

    mapperNo = (uint8_t(f7.mapNo_hi4) << 4) | f6.mapNo_lo4;

    headerSz  = 16;
    trainerSz = (f6.trainer512byte) ? 512 : 0;
    prgSz     = 16*1024 * H.prgsz;
    chrSz     =  8*1024 * H.chrsz;
    playChInstRomSz = 0;
    playChPRomSz = 0;
    titleSz = 0;

    headerOff        = 0x0000;
    trainerOff       = headerOff + headerSz;
    prgOff           = trainerOff + trainerSz;
    chrOff           = prgOff + prgSz;
    playChInstRomOff = chrOff + chrSz;
    playChPRomOff    = playChInstRomOff + playChInstRomSz;
    titleOff         = playChPRomOff + playChPRomSz;
    titleSz          = size - titleOff;

    printf("iNES::FileInfo\n");
    printf("----------------------\n");
    printf("header        %04X - %04X\n", headerOff, headerOff+headerSz);
    printf("trainer       %04X - %04X\n", trainerOff, trainerOff+trainerSz);
    printf("prgRom        %04X - %04X\n", prgOff, prgOff+prgSz);
    printf("chrRom        %04X - %04X\n", chrOff, chrOff+chrSz);
    printf("playChInstRom %04X - %04X\n", playChInstRomOff, playChInstRomOff+playChInstRomSz);
    printf("playChPRom    %04X - %04X\n", playChPRomOff, playChPRomOff+playChPRomSz);
    printf("title         %04X - %04X\n", titleOff, titleOff+titleSz);
    printf("----------------------\n\n");

    printf("iNES::FileHeader\n");
    printf("----------------------\n");
    printf("prgsz            %u\n", H.prgsz);
    printf("chrsz            %u\n", H.chrsz);
    printf("mapper           %u\n", mapperNo);

    printf("mirroringMode    %u\n", f6.mirroringMode);
    printf("batteryBackedRAM %u\n", f6.batteryBackedRAM);
    printf("trainer512byte   %u\n", f6.trainer512byte);
    printf("fourScreenVRAM   %u\n", f6.fourScreenVRAM);

    printf("ramNum8kBanks    %u\n", f8.ramNum8kBanks);
    printf("isPAL            %u\n", f9.isPAL);
    printf("----------------------\n\n");
}



NesFile::iNES::iNES( uint8_t *base, size_t size )
:   NesFile::Base(base, size),
    mInfo(base, size),
    mHead(*(FileHead*)base)
{
    auto &I = mInfo;
    mPrgROM = MemoryRO(base + I.prgOff, I.prgSz);
    mChrROM = MemoryRO(base + I.chrOff, I.chrSz);

    // mPrgROM.flash(raw + mInfo.prgRomOff, mInfo.prgRomSz);
    // mChrROM.flash(raw + mInfo.chrRomOff, mInfo.chrRomSz);
}

