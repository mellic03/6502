#include <memu/nes/cpu.hpp>
#include <memu/nes/nes.hpp>



ubyte NesCPU::PageHandlerMMIO::read(addr_t addr)
{
    ubyte *mmio = nes.mCPU.mmio;
    ubyte  data = 0;

    switch (addr)
    {
        case 0x4016: // player 1
        case 0x4017: // player 2
            data = (mmio[addr-0x4000] & 0b01);
            mmio[addr-0x4000] >>= 1;
            // printf("[read 0x%04X] bit=%u\n", addr, data);
            break;

        default:
            break;
    }

    return data;
}


void NesCPU::PageHandlerMMIO::write(addr_t addr, ubyte data)
{
    if (addr != 0x4016) // controller strobe
    {
        return;
    }

    ubyte *mmio = nes.mCPU.mmio;
    ubyte *gpad = (ubyte*)(nes.mPlayerCtl);
    ubyte  prev = (mmio[0x16] & 0b01);
    ubyte  curr = (data & 0b01);
    mmio[0x16] = curr;

    // if released, load state of all 8 buttons into shift registers
    if (prev==1 && curr==0)
    {
        mmio[0x16] = gpad[0];
        mmio[0x17] = gpad[1];
    }

    else
    {
        mmio[0x16] = curr;
    }
}

