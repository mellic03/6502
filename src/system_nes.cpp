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
    int opts = argc - 2; // name, FILE

    memu::ConfigParser conf("boot.conf");
    for (int i=1; i<argc-1; i+=2)
        if (std::string(argv[i]) == "--conf")
            conf = memu::ConfigParser(argv[i+1]);
    conf.print();
    auto &boot = conf["boot"];

    auto *nes = new NesEmu::System();
    nes->loadGamePak(new NesEmu::GamePak(boot["rom"]));
    nes->mPPU.loadPalette(conf["video"]["palette"]);

    if (boot.contains("jump"))
    {
        nes->mCPU.PC = (uint16_t)strtol(boot["jump"].c_str(), NULL, 16);
    }

    auto *fh = (NesEmu::iNES_File*)(nes->mGPak->data());
    std::string title0 = std::string("NesEmu ") + (fh->IsPAL ? "[PAL]" : "[NTSC]");

    EmuIO io;
    auto *win0   = new EmuWindow(title0.c_str(), 256+128, 240, 4);
    auto *fbgame = new EmuFramebuffer(256, 240);
    auto *fbpal  = new EmuFramebuffer(128, 256);
    auto *fbpal0 = new EmuFramebuffer(128, 128);
    auto *fbpal1 = new EmuFramebuffer(128, 128);

    int palNo = 0;
    uint64_t tcurr = SDL_GetTicks64();
    uint64_t tprev = tcurr;
    uint64_t tdiff = 0;
    uint64_t accum = 0;

    while (!nes->mCPU.mInvalidOp)
    {
        io.updateEvents();

        tcurr = SDL_GetTicks64();
        tdiff = tcurr - tprev;
        tprev = tcurr;

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
            nes->mCPU.sigHigh(m6502::PIN_RES);
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

        if (io.keyReleased(SDL_SCANCODE_LEFT))
        {
            palNo = (palNo-1) % 12;
        }

        if (io.keyReleased(SDL_SCANCODE_RIGHT))
        {
            palNo = (palNo+1) % 12;
        }

        nes->mPPU.drawPatternTable(fbpal0, palNo, {0, 0});
        nes->mPPU.drawPatternTable(fbpal1, palNo, {0, 128});
        win0->blit(fbpal0, {256, 0});
        win0->blit(fbpal1, {256, 128});
        win0->blit(fbgame, {0, 0});

        win0->flush();
    }

    return 0;
}

