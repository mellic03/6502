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
:   ioDevice(nullptr, 0),
    mPrgROM(nullptr, 0),
    mChrROM(nullptr, 0)
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
    mData = new uint8_t[mSize];
    stream.read((char*)mData, mSize);

    mFmt   = rdFmt(mData);
    miNES  = new NesFile::iNES(mData, mSize);
    mNES20 = nullptr;

    mPrgROM = MemoryRO(mData + miNES->prgOff, miNES->prgSz);
    mChrROM = MemoryRO(mData + miNES->chrOff, miNES->chrSz);
}


// ubyte NesEmu::GamePak::ioRead( uint16_t addr )
// {
//     return mBase[addr];
// }

// void NesEmu::GamePak::ioWrite( uint16_t addr, ubyte value )
// {
//     mBase[addr] = value;
//     // mData[fh.prgRomOff + addr] = byte;
// }




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