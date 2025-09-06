#pragma once

#include "../gamepak.hpp"
#include "../system.hpp"

namespace NesEmu
{
    void ExecuteMapper( int mapno, System& );
}

void Mapper00_NROM(NesEmu::System&);
void Mapper01_MMC1(NesEmu::System&);
void Mapper02_UxROM(NesEmu::System&);
void Mapper03_CNROM(NesEmu::System&);
void Mapper04_MMC3(NesEmu::System&);
