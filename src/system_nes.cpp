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
    int opts = argc - 2; // name, FILE

    printf("argc: %d\n", argc);
    if (argc<2 || (opts % 2) != 0)
    {
        printf("Usage: nesemu FILE [OPTIONS]\n\n");
        printf(" Option         Long option          Meaning\n");
        printf(" -j <addr>      --jump <addr>        Jump to address\n");
        return 1;
    }


    auto *nes = new NesEmu::System();
    nes->loadGamePak(new NesEmu::GamePak(argv[1]));
    
    for (int i=2; i<argc-1; i+=2)
    {
        if (std::string(argv[i]) == "--jump")
        {
            nes->mCPU.PC = (uint16_t)strtol(argv[i+1], NULL, 16);
        }
    }

    Display D;
    auto *win0 = D.addWindow(new EmuWindow("NES Emulation", 256, 240, 4));
    auto *win1 = D.addWindow(new EmuWindow("CHR Pattern Tables", 256, 128, 4));
    // auto *win2 = D.addWindow(new EmuWindow("CHR-ROM 2", 128, 128, 4));

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

        nes->mPPU.drawPatternTable(win1, {0, 0}, {0, 0}, {128, 128});
        nes->mPPU.drawPatternTable(win1, {128, 0}, {0, 128}, {128, 128});

        D.endFrame();
    }

    return 0;
}

