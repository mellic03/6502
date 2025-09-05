#include "cartridge.hpp"
#include "file/ines.hpp"

#include <cassert>
#include <cstring>
#include <fstream>


NesEmu::Cartridge::Cartridge( const std::string &path )
{
    std::ifstream stream(path, std::ifstream::binary);
    if (!stream.is_open())
    {
        return;
    }

    stream.seekg(0, std::ifstream::end);
    mDataSz = stream.tellg();

    stream.seekg(0, std::ifstream::beg);
    mData = std::unique_ptr<uint8_t[]>(new uint8_t[mDataSz]);
    stream.read(reinterpret_cast<char*>(mData.get()), mDataSz);

    mFormat = _readfmt(mData.get());
    mHead   = mData.get();
    mHeadSz = 0x10;

    NesFile::iNES fh(mData.get());

    auto &H = *(NesFile::iNES::header_t*)mHead;
    mPrgSz  = 16384 * H.prgNo16kBanks;
    mChrSz  = 8192  * H.chrNo8kBanks;
    mMapNo  = (uint8_t(H.f7.mapNo_hi) << 4) | H.f6.mapNo_lo;

    mHead = mData.get();
    mPrg  = mHead + mHeadSz;
    mChr  = mPrg  + mPrgSz;

    printf("signature   %s\n",  H.signature);
    printf("prgRomSize  %lu\n", mPrgSz);
    printf("chrRomSize  %lu\n", mChrSz);
    printf("mapNo       %u\n",  mMapNo);
    printf("flags6      %u\n",  H.flags6);
    printf("flags7      %u\n",  H.flags7);
    printf("flags8      %u\n",  H.flags8);
    printf("flags9      %u\n",  H.flags9);
    printf("flags10     %u\n",  H.flags10);
    printf("\n");
}



uint8_t NesEmu::Cartridge::_readfmt( uint8_t *rom )
{
    uint8_t fmt = Fmt::OTHER;

    if (strncmp("NES\x1A", (char*)rom, 4) == 0)
        fmt = Fmt::iNES;

    if (fmt==Fmt::iNES && (rom[7] & 0x0C)==0x08)
        fmt = Fmt::NES20;

    return fmt;
}

