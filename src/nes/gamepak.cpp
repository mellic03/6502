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
:   mFile(0)
{
    std::ifstream stream(path, std::ifstream::binary);
    if (!stream.is_open()) return;

    stream.seekg(0, std::ifstream::end);
    mFile.resize(stream.tellg());

    stream.seekg(0, std::ifstream::beg);
    stream.read((char*)(mFile.data()), mFile.size());

    mHeader  = (iNES_Header*)(mFile.data());
    mData    = mFile.data() + sizeof(iNES_Header);
    auto &H  = *mHeader;


    ubyte *fbeg = mFile.data();
    ubyte *fend = fbeg + mFile.size();
    ubyte *fpos = fbeg + 0x10;

    mPrgROM.resize(0x4000 * H.prgRomSz);
    std::memcpy(&mPrgROM[0], fpos, mPrgROM.size());
    fpos += mPrgROM.size();

    mChrROM.resize(0x2000 * H.chrRomSz);
    std::memcpy(&mChrROM[0], fpos, mChrROM.size());
    fpos += mChrROM.size();

    printf("Remaining: %lu\n",  fend - fpos);

    mMapNo = (uint8_t(H.MapperHi4) << 4) | H.MapperLo4;
    mMapper = NesEmu::CreateMapper(mMapNo, mFile.data());


    // mPrgROM = MemoryRO(ptr, 0x4000 * H.prgRomSz);
    // ptr += mPrgROM.size();

    // mChrROM = MemoryRO(ptr, 0x2000 * H.chrRomSz);
    // ptr += mChrROM.size();

    // mPrgRAM = MemoryRW(ptr, 0x2000 * ((H.prgRamSz==0) ? 1 : H.prgRamSz));
    // ptr += mPrgRAM.size();

    printf("iNES::iNES\n");
    printf("--------------------------------------------\n");
    printf("prgRomSz         %lu\n", mPrgROM.size());
    printf("chrRomSz         %lu\n", mChrROM.size());
    printf("mapper           %u\n",  mMapNo);

    printf("mirroringMode    %u\n", H.f6MirroringMode);
    printf("batteryBackedRAM %u\n", H.f6BatteryBackedRAM);
    printf("trainer512byte   %u\n", H.f6Trainer512byte);
    printf("fourScreenVRAM   %u\n", H.f6FourScreenVRAM);

    // printf("prgRamSz         %lu\n", mPrgRAM.size());
    printf("isPAL            %u\n",  H.f9IsPAL);
    printf("--------------------------------------------\n\n");

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