#pragma once

#include "../gamepak.hpp"
#include "../system.hpp"

namespace NesEmu
{
    void callHwMapper( int mapno, System& );
}

void HwMapper00_NROM(NesEmu::System&);
void HwMapper01_MMC1(NesEmu::System&);
void HwMapper02_UxROM(NesEmu::System&);
void HwMapper03_CNROM(NesEmu::System&);
