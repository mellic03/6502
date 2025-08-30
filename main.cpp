#include <stdio.h>
#include <assert.h>

#include <stddef.h>
#include <stdint.h>

#include "6502.hpp"




int main( int argc, char **argv )
{
    static uint8_t prog[1024];
    prog[0] = 0x01;
    prog[1] = 0x11;
    prog[2] = 0x21;
    prog[3] = 0x31;
    prog[4] = 0xFF;

    cpu6502 cpu;

    cpu.LoadProgram(prog, 1024);
    cpu.Run();

    // prog[0] = 0x06;

    // cpu_6502 cpu(prog);

    // while (cpu.mRunning)
    // {
    //     uint8_t *ptr = cpu.fetch();
    //     cpu_6502_inst inst = cpu.decode(ptr);

    //     if (!cpu.execute(inst))
    //     {
    //         return 1;
    //     }
    // }

    return 0;
}

