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
static EmuImageFont *font;
static NesEmu::System *nes;

#define VA_ARGS(...) , ##__VA_ARGS__


int main( int argc, char **argv )
{
    srand(clock());

    emuio = new EmuIO();
    font  = new EmuImageFont("font/SpaceMonoBold-w10h15f12.bmp");
    nes   = new NesEmu::System(emuio);
    nes->loadGamePak(new NesEmu::GamePak(nes->mConf["boot"]["rom"]));

    NesTest::compare("nestest-data.log", nes->mCPU);
    return 0;

    auto *gwin = nes->mGameWin;
    auto *cwin = nes->mChrWin;
    auto *iwin = emuio->makeWin("Debug", 256, 256, 2, 1024);
          iwin->setScaleMode(SDL_SCALEMODE_LINEAR);

    int count = 0;

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

    
        iwin->mOnUpdate = [](EmuWindow *W) {
            #define wprintf(Fmt, ...) W->print(font, Fmt VA_ARGS(__VA_ARGS__));

            W->setBounds(0, 0, 128, 256);
            wprintf("CPU\n");
            wprintf("accum %d\n", nes->mCycleAccum);
            wprintf("wait  %u\n", nes->mCPU.mWaiting);

            W->setBounds(128, 0, 128, 256);
            wprintf("PPU\n");
            wprintf("line  %d\n", nes->mPPU.mScanLine);
            wprintf("cycle %d\n", nes->mPPU.mCycle);
            wprintf("addr  %04X\n", nes->mPPU.ppuaddr);
            wprintf("data  %02X\n", nes->mPPU.ppudata);
            // gwin->print(font, "PC   %04X\n", nes->mCPU.PC);
            // gwin->print(font, "AC   %02X\n", nes->mCPU.AC);
            // gwin->print(font, "XR   %02X\n", nes->mCPU.XR);
            // gwin->print(font, "YR   %02X\n", nes->mCPU.YR);


            // uword SP = nes->mCPU.SP;
            // uword count = 0;

            // wprintf("SP   ");
            // SP = nes->mCPU.SP;
            // count = 0;
            // while (SP <= 0xFF && count++ < 10)
            //     wprintf("%02X ", SP++);
            // wprintf("\n");

            // wprintf("     ");
            // SP  = nes->mCPU.SP;
            // count = 0;
            // while (SP <= 0xFF && count++ < 10)
            //     wprintf("%02X ", nes->mCPU.rdbus(SP++));
            // wprintf("\n");

            // wprintf("SSR  N V B U D I Z C\n");
            // wprintf("     ");
            // for (int i=7; i>=0; i--)
            //     wprintf("%u ", (nes->mCPU.SSR.byte & (1<<i)) ? 1 : 0);
            // wprintf("\n");

            // gwin->setBounds(512, 32, 1024, 512);
            // wprintf("PPUSTAT %02X\n", nes->mPPU.ppustat.byte);
            // wprintf("PPUADDR %04X\n", nes->mPPU.ppuaddr);
            // wprintf("PPUDATA %02X\n", nes->mPPU.ppudata);

            #undef wprintf
        };
    
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
            nes->ioLineIRQ = 1;
            break;

        case SDLK_R:
            printf("Key R --> RESET\n");
            nes->ioLineRES = 1;
            break;

        case SDLK_N:
            printf("Key N --> NMI\n");
            nes->ioLineNMI = 1;
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
