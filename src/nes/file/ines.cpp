#include "ines.hpp"
#include <cstring>
using namespace NesFile;


NesFile::iNES::iNES( uint8_t *base )
:   data(base)
{
    auto &H  = *(iNES::FileHeader*)(data);
    auto &f6 = H.f6;
    auto &f7 = H.f7;

    headerSz  = 16;
    trainerSz = (f6.trainer512byte) ? 512 : 0;
    prgRomSz  = 16*1024 * H.prgNo16kBanks;
    chrRomSz  =  8*1024 * H.chrNo8kBanks;
    playChoiceInstRomSz = 0;
    playChoicePRomSz = 0;
    titleSz = 0;

    mapperNo = (uint8_t(f7.mapNo_hi4) << 4) | f6.mapNo_lo4;
}

