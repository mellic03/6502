#include "mapper.hpp"
#include <cassert>

using namespace NesEmu;
using MapperFunc = void (*)(NesEmu::System&);

static MapperFunc MapTable[]
{
    Mapper00_NROM,
    Mapper01_MMC1,
    Mapper02_UxROM,
    Mapper03_CNROM,
    Mapper04_MMC3,
};

void NesEmu::ExecuteMapper( int mapno, NesEmu::System &nes )
{
    assert(0<=mapno && mapno<=4);
    MapTable[mapno](nes);
}


