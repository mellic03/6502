#include "mapper.hpp"
using namespace NesEmu;




/*
    Banks
    --------------------------------------------------------------------------------------------
    - CPU  6000-7FFF: 8 KB PRG RAM bank (optional).

    - CPU  8000-9FFF: 8 KB switchable PRG ROM bank.
       OR  C000-DFFF

    - CPU  A000-BFFF: 8 KB switchable PRG ROM bank.

    - CPU  C000-DFFF: 8 KB PRG ROM bank, fixed to the second-last bank
       OR  8000-9FFF

    - CPU  E000-FFFF: 8 KB PRG ROM bank, fixed to the last bank
    - PPU  0000-07FF (or $1000-$17FF): 2 KB switchable CHR bank
    - PPU  0800-0FFF (or $1800-$1FFF): 2 KB switchable CHR bank
    - PPU  1000-13FF (or $0000-$03FF): 1 KB switchable CHR bank
    - PPU  1400-17FF (or $0400-$07FF): 1 KB switchable CHR bank
    - PPU  1800-1BFF (or $0800-$0BFF): 1 KB switchable CHR bank
    - PPU  1C00-1FFF (or $0C00-$0FFF): 1 KB switchable CHR bank

*/

void Mapper04_MMC3( System &nes )
{

}

