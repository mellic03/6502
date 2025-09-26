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


static EmuIO *io;
static NesEmu::System *nes;

int main( int argc, char **argv )
{
    srand(clock());

    nes = new NesEmu::System();
    io  = &(nes->mEmuIO);

    auto *gwin = nes->mGameWin;
    auto *cwin = nes->mChrWin;
    auto &conf = nes->mConf;

    nes->loadGamePak(new NesEmu::GamePak(conf["boot"]["rom"]));

    // ubyte A = nes->mBusCPU.read(0xFFFA);
    // ubyte B = nes->mBusCPU.read(0xFFFB);
    // ubyte C = nes->mBusCPU.read(0xFFFC);
    // ubyte D = nes->mBusCPU.read(0xFFFD);
    // ubyte E = nes->mBusCPU.read(0xFFFE);
    // ubyte F = nes->mBusCPU.read(0xFFFF);
    // printf("%02X %02X %02X %02X %02X %02X\n", A, B, C, D, E, F);
    // exit(1);

    while (io->mRunning)
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
            if (e.type == SDL_EVENT_GAMEPAD_ADDED)
            {
                SDL_JoystickID jid = e.gdevice.which;
                SDL_Gamepad *pad = SDL_OpenGamepad(jid);
            }

            NesEmu_PlayerCtl(&e);
            NesEmu_HandleEvent(&e);
        }

        io->update();
    
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
            io->quit();
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
            io->quit();
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
