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
    auto *win0  = new EmuWindow("NesEmu", 256+128, 240, 4);
    auto  pat0  = new EmuFramebuffer(128, 128);
    auto  pat1  = new EmuFramebuffer(128, 128);
    auto  gview = new EmuFramebuffer(256, 240);
    // uint64_t tcurr = SDL_GetTicksNS();
    // uint64_t tprev = tcurr;
    // uint64_t tdiff = 0;
    // uint64_t accum = 0;

    while (!nes->mCPU.mInvalidOp)
    {
        using namespace memu;

        io.updateEvents();

        // tcurr = SDL_GetTicksNS();
        // tdiff = tcurr - tprev;
        // tprev = tcurr;
        nes->tick();

        if (io.keyReleased(SDL_SCANCODE_SPACE))
        {
            printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
            nes->mCPU.wait();
        }

        if (io.keyReleased(SDL_SCANCODE_I))
        {
            printf("Key I --> IRQ\n");
            nes->mBusCPU.pend_irq = 1;
            // nes->mBusCPU.sigSet(SIG::IRQ, 0);
        }

        if (io.keyReleased(SDL_SCANCODE_R))
        {
            // printf("Key R --> RESET\n");
            // nes->mBusCPU.sigSet(SIG::RES, 0);
        }

        if (io.keyReleased(SDL_SCANCODE_N))
        {
            printf("Key N --> NMI\n");
            nes->mBusCPU.pend_nmi = 1;
            // nes->mBusCPU.sigFlip(SIG::NMI);
        }

        if (io.keyReleased(SDL_SCANCODE_ESCAPE))
        {
            SDL_Quit();
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


        static int count = 0;
        static ubyte prev = 0;
        ubyte curr = nes->mPPU.STATUS.V;

        if (prev==0 && curr==1)
        {
            for (int i=0; i<16; i++)
            {
                for (int j=0; j<16; j++)
                {
                    nes->mPPU.drawPattern(pat0, 0, 8*j, 8*i, j, i);
                    nes->mPPU.drawPattern(pat1, 1, 8*j, 8*i, j, i);
                }
            }

            nes->mPPU.drawNameTable(win0, 0x2000);

            win0->blit(pat0, 256, 0*128);
            win0->blit(pat1, 256, 1+128);
            win0->flush();
        }

        prev = curr;
    }

    return 0;
}

