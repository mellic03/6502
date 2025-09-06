#include "hwmapper.hpp"
#include <cassert>

using namespace NesEmu;
using HwMapperFunc = void (*)(NesEmu::System&);

static HwMapperFunc HwMapTable[]
{
    HwMapper00_NROM,
    HwMapper01_MMC1,
    HwMapper02_UxROM,
    HwMapper03_CNROM
};


void NesEmu::callHwMapper( int idx, NesEmu::System &nes )
{
    using Fmt = NesEmu::GamePak::Fmt;
    assert(0<=idx && idx<=3);
    (HwMapTable[idx])(nes);
}


