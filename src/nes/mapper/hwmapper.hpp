#pragma once

#include "../cartridge.hpp"
#include "../system.hpp"


namespace NesEmu
{
    class System;
    class Cartridge;
    struct HwMapper { virtual void map( System&, Cartridge* ) = 0; };

    HwMapper *getMapper( int mapno );
}



#define HW_MAPPER(Nm) \
struct HwMapper##Nm: public HwMapper \
{ virtual void map( System&, Cartridge* ) final; };

namespace NesEmu
{
    HW_MAPPER(00_NROM);
    HW_MAPPER(01_MMC1);
    HW_MAPPER(02_UxROM);
    HW_MAPPER(03_CNROM);
}
