#include "hwmapper.hpp"
#include <cassert>


static NesEmu::HwMapper **InitHwMappers()
{
    using namespace NesEmu;
    using Fmt = NesEmu::Cartridge::Fmt;

    auto **ftab = new HwMapper*[Fmt::NumFmt];
    ftab[Fmt::iNES]  = new HwMapper00_NROM();
    ftab[Fmt::NES20] = new HwMapper01_MMC1();
    ftab[Fmt::OTHER] = new HwMapper02_UxROM();
    return ftab;
}
static NesEmu::HwMapper **HwMapTable = InitHwMappers();



NesEmu::HwMapper *NesEmu::getMapper( int idx )
{
    using Fmt = NesEmu::Cartridge::Fmt;
    assert(0<=idx && idx<=Fmt::NumFmt);
    return HwMapTable[idx];
}


