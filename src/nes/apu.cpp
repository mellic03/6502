#include "apu.hpp"


NesAPU::NesAPU()
:   ioDevice(new MemoryPage())
{
    mMMIO = data<ApuMMIO>();
}

