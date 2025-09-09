#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <memu/addrspace.hpp>
#include <memu/hw/mos6502.hpp>


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        printf("Usage: assembler [filepath]\n");
        return 1;
    }

    // NesEmu::GamePak gpak(argv[1]);
    // if (gpak.data() == nullptr)
    // {
    //     printf("Could not open file \"%s\"\n", argv[1]);
    //     return 1;
    // }

    auto bus = memu::AddrSpace();
    auto cpu = m6502(bus);

    while (cpu.mInvalidOp && cpu.mOpCount < 1000)
    {
        bus.tick();
    }

    return 0;
}

