#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <memu/display.hpp>
#include <memu/nes/mapper.hpp>
#include <memu/nes/nes.hpp>


int main( int argc, char **argv )
{
    NesEmu::System *nes = new NesEmu::System();

    for (int i=1; i<argc-1; i+=2)
    {
        if (0)
        {

        }
        
        else if (std::string(argv[i]) == "--rom")
        {
            nes->LoadROM(new NesEmu::GamePak(argv[i+1]));
        }

        else if (std::string(argv[i]) == "--jump")
        {
            nes->mCPU.PC = (uint16_t)strtol(argv[i+1], NULL, 16); 
        }
    }

    if (argc==1 || !nes->mGPak)
    {
        printf("Usage: memu-system-nes --rom [filepath] --jump [address]\n");
        return 1;
    }

    Display D;
    D.init(256, 240, 4);

    uint64_t tcurr = SDL_GetTicks64();
    uint64_t tprev = tcurr;
    uint64_t tdiff = 0;
    uint64_t accum = 0;

    while (!nes->mCPU.mInvalidOp)
    {
        D.beginFrame();

        tcurr = SDL_GetTicks64();
        tdiff = tcurr - tprev;
        tprev = tcurr;

        nes->tick();

        if (D.keyReleased(SDL_SCANCODE_SPACE))
        {
            printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
            nes->mCPU.sigFlip(m6502::PIN_WAI);
        }

        if (D.keyReleased(SDL_SCANCODE_I))
        {
            printf("Key I --> IRQ\n");
            nes->mCPU.SSR.I = 0;
            nes->mCPU.sigLow(m6502::PIN_IRQ);
        }

        if (D.keyReleased(SDL_SCANCODE_R))
        {
            printf("Key R --> RESET\n");
            nes->mCPU.sigHigh(m6502::PIN_RES);
        }

        if (D.keyReleased(SDL_SCANCODE_N))
        {
            printf("Key N --> NMI\n");
            nes->mCPU.sigFlip(m6502::PIN_NMI);
        }

        if (D.keyReleased(SDL_SCANCODE_ESCAPE))
        {
            break;
        }

        D.endFrame();
    }

    return 0;
}

