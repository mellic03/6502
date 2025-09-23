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


static void NesEmu_HandleEvent( SDL_Event *e );


static EmuIO io;
static NesEmu::System *nes;

int main( int argc, char **argv )
{
    srand(clock());

    nes = new NesEmu::System();
    auto &conf = NesEmu::CONF;

    nes->loadGamePak(new NesEmu::GamePak(conf["boot"]["rom"]));
    nes->mPPU.loadPalette(conf["video"]["palette"]);

    if (conf["boot"]["jump"])
    {
        // nes->mCPU.PC = (uint16_t)strtol(conf["boot"]["jump"], NULL, 16);
    }

    auto *win0 = new EmuWindow("NesEmu", 256, 240, 4);
    auto *win1 = new EmuWindow("CHR-ROM", 128, 256, 4);
    // uint64_t tcurr = SDL_GetTicksNS();
    // uint64_t tprev = tcurr;
    // uint64_t tdiff = 0;
    // uint64_t accum = 0;

    // SDL_LoadBMP("data/font/atlas.png");

    while (io.mRunning)
    {
        using namespace memu;

        nes->tick();

        if (nes->mCPU.mInvalidOp)
        {
            break;
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
                    nes->mPPU.drawPattern(win1, 0, 8*j,     8*i, i, j);
                    nes->mPPU.drawPattern(win1, 1, 8*j, 128+8*i, i, j);
                }
            }
            win1->flush();

            nes->mPPU.drawNameTable(win0, 0, 0, 0x2000);
            win0->flush();
        }
        prev = curr;


        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            NesEmu_HandleEvent(&e);
        }

        SDL_PumpEvents();
    }

    return 0;
}




static void NesEmu_HandleEvent( SDL_Event *e )
{
    switch (e->type)
    {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            io.quit();
        default:
            break;
    }

    if (e->type != SDL_EVENT_KEY_UP)
    {
        return;
    }

    switch (e->key.key)
    {
        case SDLK_ESCAPE:
            io.quit();
            break;

        case SDLK_SPACE:
            printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
            nes->mCPU.wait();
            break;

        case SDLK_I:
            printf("Key I --> IRQ\n");
            nes->mBusCPU.pend_irq = 1;
            break;

        case SDLK_R:
            printf("Key R --> RESET\n");
            break;

        case SDLK_N:
            printf("Key N --> NMI\n");
            nes->mBusCPU.pend_nmi = 1;
            break;

        case SDLK_TAB:
            break;

        case SDLK_LEFT:
            if (--nes->mPPU.mPalNo < 0)
                nes->mPPU.mPalNo = 12;
            break;

        case SDLK_RIGHT:
            if (++nes->mPPU.mPalNo > 12)
                nes->mPPU.mPalNo = 0;
            break;

        default:
            break;
    }

}



static void SDL_AppQuit( void *appstate, SDL_AppResult result )
{

}
