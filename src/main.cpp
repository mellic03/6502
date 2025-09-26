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


static EmuIO *emuio;
static NesEmu::System *nes;

#define VA_ARGS(...) , ##__VA_ARGS__
#define gwprintf(Fmt, ...) gwin->print(font, Fmt VA_ARGS(__VA_ARGS__));


int main( int argc, char **argv )
{
    srand(clock());

    emuio = new EmuIO();
    nes   = new NesEmu::System(emuio);
    nes->loadGamePak(new NesEmu::GamePak(nes->mConf["boot"]["rom"]));

    auto *gwin = nes->mGameWin;
    auto *cwin = nes->mChrWin;
    auto *font = new EmuImageFont("font/cutive-w12hf18.bmp");

    while (emuio->running())
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
        SDL_PumpEvents();

        // {
        //     gwin->setBounds(32, 32, 512, 512);
        //     gwin->print(font, "PC   %04X\n", nes->mCPU.PC);
        //     gwin->print(font, "AC   %02X\n", nes->mCPU.AC);
        //     gwin->print(font, "XR   %02X\n", nes->mCPU.XR);
        //     gwin->print(font, "YR   %02X\n", nes->mCPU.YR);


        //     uword SP = nes->mCPU.SP;
        //     uword count = 0;

        //     gwprintf("SP   ");
        //     SP = nes->mCPU.SP;
        //     count = 0;
        //     while (SP <= 0xFF && count++ < 10)
        //         gwprintf("%02X ", SP++);
        //     gwprintf("\n");

        //     gwprintf("     ");
        //     SP  = nes->mCPU.SP;
        //     count = 0;
        //     while (SP <= 0xFF && count++ < 10)
        //         gwprintf("%02X ", nes->mCPU.rdbus(SP++));
        //     gwprintf("\n");

        //     gwprintf("SSR  N V B U D I Z C\n");
        //     gwprintf("     ");
        //     for (int i=7; i>=0; i--)
        //         gwprintf("%u ", (nes->mCPU.SSR.byte & (1<<i)) ? 1 : 0);
        //     gwprintf("\n");

        //     gwin->setBounds(512, 32, 1024, 512);
        //     gwprintf("PPUSTAT %02X\n", nes->mPPU.ppustat.byte);
        //     gwprintf("PPUADDR %04X\n", nes->mPPU.ppuaddr);
        //     gwprintf("PPUDATA %02X\n", nes->mPPU.ppudata);
        // }
    
        emuio->update();
    }

    SDL_Quit();

    return 0;
}



static void NesEmu_HandleEvent( SDL_Event *e )
{
    switch (e->type)
    {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            emuio->quit();
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
            emuio->quit();
            break;

        case SDLK_SPACE:
            printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
            nes->mCPU.wait();
            break;

        case SDLK_I:
            printf("Key I --> IRQ\n");
            nes->ioLineIRQ |= 0xFF;
            break;

        case SDLK_R:
            printf("Key R --> RESET\n");
            nes->ioLineRES |= 0xFF;
            break;

        case SDLK_N:
            printf("Key N --> NMI\n");
            nes->ioLineNMI |= 0xFF;
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
