#include "gamepak.hpp"
#include "file/ines.hpp"

#include <cassert>
#include <cstring>
#include <fstream>


static uint8_t rdFmt( uint8_t *rom )
{
    using Fmt = NesEmu::GamePak::Fmt;
    uint8_t fmt = Fmt::OTHER;

    if (strncmp("NES\x1A", (char*)rom, 4) == 0)
        fmt = Fmt::iNES;

    if (fmt==Fmt::iNES && (rom[7] & 0x0C)==0x08)
        fmt = Fmt::NES20;

    return fmt;
}


NesEmu::GamePak::GamePak( const std::string &path )
{
    std::ifstream stream(path, std::ifstream::binary);

    if (!stream.is_open())
    {
        mData = nullptr;
        return;
    }

    stream.seekg(0, std::ifstream::end);
    mSize = stream.tellg();

    stream.seekg(0, std::ifstream::beg);
    mData = std::unique_ptr<uint8_t[]>(new uint8_t[mSize]);
    mBase = mData.get();
    stream.read(reinterpret_cast<char*>(mBase), mSize);

    mFmt   = rdFmt(mBase);
    miNES  = new NesFile::iNES(mBase, mSize);
    mNES20 = nullptr;

    // mHead   = mData.get();
    // mHeadSz = 0x10;
    // NesFile::iNES fh(mData.get());

    // auto &H = *(NesFile::iNES::header_t*)mHead;
    // mPrgSz  = 16384 * H.prgNo16kBanks;
    // mChrSz  = 8192  * H.chrNo8kBanks;
    // mMapNo  = (uint8_t(H.f7.mapNo_hi) << 4) | H.f6.mapNo_lo;

    // mHead = mData.get();
    // mPrg  = mHead + mHeadSz;
    // mChr  = mPrg  + mPrgSz;

    // printf("signature   %s\n",  H.signature);
    // printf("prgRomSize  %lu\n", mPrgSz);
    // printf("chrRomSize  %lu\n", mChrSz);
    // printf("mapNo       %u\n",  mMapNo);
    // printf("flags6      %u\n",  H.flags6);
    // printf("flags7      %u\n",  H.flags7);
    // printf("flags8      %u\n",  H.flags8);
    // printf("flags9      %u\n",  H.flags9);
    // printf("flags10     %u\n",  H.flags10);
    // printf("\n");
}


uint8_t NesEmu::GamePak::rd( uint16_t addr )
{
    return mBase[addr];
}

void NesEmu::GamePak::wt( uint16_t addr, uint8_t byte )
{
    // mData[fh.prgRomOff + addr] = byte;
}




// NesEmu::FileU::FileU( uint8_t *raw, size_t size )
// :   fmt(rdFmt(raw))
// {
//     using Fmt = NesEmu::GamePak::Fmt;
//     namespace NF = NesFile;

//     switch (fmt)
//     {
//         case Fmt::iNES:
//             as_iNES  = NF::iNES(raw, size);
//             break;

//         case Fmt::NES20:
//             as_NES20 = NF::NES20(raw, size);
//             break;

//         default:
//             fprintf(stderr, "[NesEmu::FileU] Bad format %u\n", fmt);
//             fprintf(stderr, "(%s:%d)\n\n", __FILE__, __LINE__);
//             exit(1);
//             break;
//     }
// }