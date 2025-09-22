#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <memu/configparser.hpp>
#include <memu/display.hpp>
#include <memu/nes/mapper.hpp>
#include <memu/nes/nes.hpp>


int main( int argc, char **argv )
{
    srand(clock());

    int opts = argc - 2; // name, FILE

    // memu::ConfigParser conf("boot.conf");
    // for (int i=1; i<argc-1; i+=2)
    //     if (std::string(argv[i]) == "--conf")
    //         conf = memu::ConfigParser(argv[i+1]);

    auto *nes = new NesEmu::System();
    auto &conf = NesEmu::CONF;

    auto ree = conf["boot"];
    auto roo = ree["rom"];

    nes->loadGamePak(new NesEmu::GamePak(conf["boot"]["rom"]));
    nes->mPPU.loadPalette(conf["video"]["palette"]);

    if (conf["boot"]["jump"])
    {
        // nes->mCPU.PC = (uint16_t)strtol(conf["boot"]["jump"], NULL, 16);
    }

    EmuIO io;
    auto *win0  = new EmuWindow("NesEmu", 256+128, 240, 2);
    auto  pat0  = new EmuFramebuffer(128, 128);
    auto  pat1  = new EmuFramebuffer(128, 128);
    auto  gview = new EmuFramebuffer(256, 240);
    // uint64_t tcurr = SDL_GetTicksNS();
    // uint64_t tprev = tcurr;
    // uint64_t tdiff = 0;
    // uint64_t accum = 0;

    while (!nes->mCPU.mInvalidOp)
    {
        io.updateEvents();

        // tcurr = SDL_GetTicksNS();
        // tdiff = tcurr - tprev;
        // tprev = tcurr;
        nes->tick();

        if (io.keyReleased(SDL_SCANCODE_SPACE))
        {
            printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
            nes->mCPU.sigFlip(m6502::PIN_WAI);
        }

        if (io.keyReleased(SDL_SCANCODE_I))
        {
            printf("Key I --> IRQ\n");
            nes->mCPU.SSR.I = 0;
            nes->mCPU.sigLow(m6502::PIN_IRQ);
        }

        if (io.keyReleased(SDL_SCANCODE_R))
        {
            printf("Key R --> RESET\n");
            nes->mCPU.sigLow(m6502::PIN_RES);
        }

        if (io.keyReleased(SDL_SCANCODE_N))
        {
            printf("Key N --> NMI\n");
            nes->mCPU.sigFlip(m6502::PIN_NMI);
        }

        if (io.keyReleased(SDL_SCANCODE_ESCAPE))
        {
            break;
        }

        if (io.keyReleased(SDL_SCANCODE_TAB))
        {
            // break;
        }

        if (io.keyReleased(SDL_SCANCODE_LEFT))
        {
            if (--nes->mPPU.mPalNo < 0)
                nes->mPPU.mPalNo = 12;
        }

        if (io.keyReleased(SDL_SCANCODE_RIGHT))
        {
            if (++nes->mPPU.mPalNo > 12)
                nes->mPPU.mPalNo = 0;
        }

        // nes->mPPU.drawPatternTable(win0, palNo, {256, 0}, {0, 0});
        // nes->mPPU.drawPatternTable(win0, palNo, {256, 128}, {0, 128});

        for (int i=0; i<16; i++)
        {
            for (int j=0; j<16; j++)
            {
                nes->mPPU.drawPattern(pat0, 0, 8*j, 8*i, j, i);
                nes->mPPU.drawPattern(pat1, 1, 8*j, 8*i, j, i);
            }
        }

        win0->blit(pat0, 256, 0*128);
        win0->blit(pat1, 256, 1+128);

        nes->mPPU.drawNameTable(win0, 0x2000, 0, 0);
        nes->mPPU.drawNameTable(win0, 0x2400, 128, 0);
        // nes->mPPU.drawNameTable(win0, 0x2800, 0, 120);
        // nes->mPPU.drawNameTable(win0, 0x2C00, 128, 120);

        win0->flush();
    }

    return 0;
}

