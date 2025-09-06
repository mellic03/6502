#include "ppu.hpp"
#include <stdio.h>


NesPPU::NesPPU()
{
    wtmmio(Reg::PPUCTRL,   11);
    wtmmio(Reg::PPUMASK,   22);
    wtmmio(Reg::PPUSTATUS, 33);
    wtmmio(Reg::OAMADDR,   44);
}


void NesPPU::tick()
{
    // printf("[PPU]\n");
}

