#include "types.hpp"
using namespace NesEmu;

void Mapper02_UxROM( System &nes )
{

}

ubyte Mapper002_UxROM::CpuAccess::read( uword addr )
{
    auto &cpu = nes.mCPU;

    if (0x0000<=addr && addr<=0x1FFF)
    {
        ubyte *cpuram = cpu.mRAM.data();
        return cpuram[addr % 2048];
    }

    if (0x2000<=addr && addr<=0x3FFF)
    {
        // return read_ppu(addr);
    }

    if (0x4000<=addr && addr<=0x4015)
    {
        return 0;
    }

    if (0x4016<=addr && addr<=0x4017)
    {
        ubyte *mmio = cpu.mMMIO;
        ubyte data = (mmio[addr-0x4000] & 0b01);
        mmio[addr-0x4000] >>= 1;
        return data;
    }

    // if (0x4018<=addr && addr<=0x5FFF)
    // {
    //     return 0;
    // }

    if (0x6000<=addr && addr<=0x7FFF)
    {
        // ubyte *ram  = uxrom.mPrgRam.base;
        // size_t size = uxrom.mPrgRam.size;
        // return ram[(addr - 0x6000) % size];
    }

    if (0x8000<=addr && addr<=0xBFFF)
    {
        // return mPrgRom0.
    }

    if (0xC000<=addr && addr<=0xFFFF)
    {
        
    }

    printf("[UxROM::CpuAccess::read] addr=0x%04X\n", addr);
    assert(false);
    return 0;
}

void Mapper002_UxROM::CpuAccess::write( uword addr, ubyte data )
{
    if (0x8000<=addr && addr<=0xBFFF)
    {
        // bank switch
    }
}
