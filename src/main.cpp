#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "hw/display.hpp"
#include "nes/gamepak.hpp"
#include "nes/system.hpp"


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        printf("Usage: assembler [filepath]\n");
        return 1;
    }

    NesEmu::GamePak gpak(argv[1]);
    if (gpak.is_bad())
    {
        printf("Could not open file \"%s\"\n", argv[1]);
        return 1;
    }

    Display D;
    D.init(256, 240, 4);

    uint64_t tcurr = SDL_GetTicks64();
    uint64_t tprev = tcurr;
    uint64_t tdiff = 0;
    uint64_t accum = 0;

    
    NesEmu::System *nes = new NesEmu::System();
    nes->LoadROM(&gpak);
    nes->mCPU.PC = 0xC000;

    while (!nes->mCPU.mInvalidOp)
    {
        D.beginFrame();

        tcurr = SDL_GetTicks64();
        tdiff = tcurr - tprev;
        tprev = tcurr;

        nes->tick(tdiff);

        if (D.keyReleased(SDL_SCANCODE_I))
        {
            printf("Key I --> IRQ\n");
            nes->mCPU.sig_irq();
        }

        if (D.keyReleased(SDL_SCANCODE_R))
        {
            printf("Key R --> RESET\n");
            nes->mCPU.sig_res();
        }

        if (D.keyReleased(SDL_SCANCODE_N))
        {
            printf("Key N --> NMI\n");
            nes->mCPU.sig_nmi();
        }

        if (D.keyReleased(SDL_SCANCODE_ESCAPE))
        {
            break;
        }

        D.endFrame();
    }

    return 0;
}

