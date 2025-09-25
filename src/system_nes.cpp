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
static void NesEmu_PlayerCtl( SDL_Event *e );


static EmuIO io;
static NesEmu::System *nes;

int main( int argc, char **argv )
{
    srand(clock());

    nes = new NesEmu::System();
    auto *win0 = nes->mWin;
    auto &conf = NesEmu::CONF;

    nes->loadGamePak(new NesEmu::GamePak(conf["boot"]["rom"]));
    nes->mPPU.loadPalette(conf["video"]["palette"]);

    // auto *win1 = new EmuWindow("CHR-ROM", 128, 256, 4);
    // SDL_LoadBMP("data/font/atlas.png");

    // uint64_t tcurr = SDL_GetTicksNS();
    // uint64_t tprev = tcurr;
    // uint64_t tdiff = 0;
    // uint64_t accum = 0;

    while (io.mRunning)
    {
        using namespace memu;

        nes->tick();

        if (nes->mCPU.mInvalidOp)
        {
            break;
        }

        // for (int i=0; i<16; i++)
        // {
        //     for (int j=0; j<16; j++)
        //     {
        //         nes->mPPU.drawPattern(win1, 0, 8*j,     8*i, j, i);
        //         nes->mPPU.drawPattern(win1, 1, 8*j, 128+8*i, j, i);
        //     }
        // }
        // win1->flush();


        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_GAMEPAD_ADDED)
            {
                SDL_JoystickID jid = e.gdevice.which;
                SDL_Gamepad *pad = SDL_OpenGamepad(jid);
            }

            NesEmu_PlayerCtl(&e);
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



static void NesEmu_PlayerCtl( SDL_Event *e )
{
    switch (e->type)
    {
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            break;
        default:
            return;
    }

    auto &ctl = nes->mPlayerCtl[0];
    bool down = e->gbutton.down;

    switch (e->gbutton.button)
    {
        default: break;
        case SDL_GAMEPAD_BUTTON_SOUTH:      ctl.a      = down; break;
        case SDL_GAMEPAD_BUTTON_EAST:       ctl.b      = down; break;
        case SDL_GAMEPAD_BUTTON_GUIDE:      ctl.sel    = down; break;
        case SDL_GAMEPAD_BUTTON_START:      ctl.start  = down; break;
        case SDL_GAMEPAD_BUTTON_DPAD_UP:    ctl.up     = down; break;
        case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  ctl.down   = down; break;
        case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  ctl.left   = down; break;
        case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: ctl.right  = down; break;
    }
}



static void SDL_AppQuit( void *appstate, SDL_AppResult result )
{

}
