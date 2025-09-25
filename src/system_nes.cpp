#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <memu/configparser.hpp>
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
    // SDL_LoadBMP("data/font/atlas.png");

    while (io.mRunning)
    {
        using namespace memu;

        nes->tick();
        if (nes->mCPU.mInvalidOp)
        {
            break;
        }

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
            nes->reset();
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
