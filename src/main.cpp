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


int main( int argc, char **argv )
{
    srand(clock());

    emuio = new EmuIO();
    font  = new EmuImageFont("font/SpaceMonoBold-w10h15f12.bmp");
    nes   = new NesEmu::System(emuio);

    // NesTest::compare("nestest-data.log", nes->mCPU);
    // return 0;

    auto *gwin = nes->mGameWin;

    if (std::string(nes->mConf["debug"]["ShowChrRom"]) == "1")
    {
        auto *cwin = emuio->makeWin("CHR-ROM", 128, 256, 4, 1024);
        cwin->mOnUpdate = [](EmuWindow *W) {
            nes->mPPU.preRenderChrRom(W);
        };
    }

    if (std::string(nes->mConf["debug"]["ShowRegs"]) == "1")
    {
        auto *iwin = emuio->makeWin("Debug", 256, 256, 2, 1024);
        iwin->setScaleMode(SDL_SCALEMODE_LINEAR);
        iwin->mOnUpdate = [](EmuWindow *W) {
            #define wprintf_va_args(...) , ##__VA_ARGS__
            #define wprintf(Fmt, ...) W->print(font, Fmt wprintf_va_args(__VA_ARGS__));
            W->setBounds(0, 0, 128, 256);
            wprintf("CPU\n");
            wprintf("time  %lu\n", nes->mCPU.clockTime());
            wprintf("PC    %04X\n", nes->mCPU.PC);
            wprintf("OP    %02X\n", nes->mCPU.mCurrOp);
            wprintf("AC    %02X\n", nes->mCPU.AC);
            wprintf("XR    %02X\n", nes->mCPU.XR);
            wprintf("YR    %02X\n", nes->mCPU.YR);
            wprintf("SP    %02X\n", nes->mCPU.SP);
            wprintf("accum %d\n", nes->mCycleAccum);
            wprintf("wait  %u\n", nes->mCPU.mWaiting);

            wprintf("SSR   NVBUDIZC\n");
            wprintf("      ");
            for (int i=7; i>=0; i--)
                wprintf("%u", (nes->mCPU.SSR.byte & (1<<i)) ? 1 : 0);
            wprintf("\n");

            wprintf("CTL0  RLDUSOAB\n");
            wprintf("      ");
            ubyte ctl0 = *(ubyte*)(nes->mPlayerCtl);
            for (int i=7; i>=0; i--)
                wprintf("%u", bool(ctl0 & (1<<i)));
            wprintf("\n");

            W->setBounds(128, 0, 128, 256);
            wprintf("PPU\n");
            wprintf("time  %lu\n", nes->mPPU.clockTime());
            wprintf("ctrl  %02X\n", nes->mPPU.ppuctl);
            wprintf("mask  %02X\n", nes->mPPU.ppumask);

            auto stat = nes->mPPU.ppustat;
            wprintf("stat  VHO00000\n");
            wprintf("      %u%u%u00000\n", stat.VBlank, stat.SpriteHit, stat.SpriteOverflow);
            wprintf("addr  %04X\n", nes->mPPU.ppuaddr);
            wprintf("data  %02X\n", nes->mPPU.ppudata);
            wprintf("line  %d\n", nes->mPPU.mScanLine);
            wprintf("cycle %d\n", nes->mPPU.mCycle);
            #undef wprintf_va_args
            #undef wprintf
        };
    }

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

        // case SDLK_SPACE:
        //     printf("Key SPACE --> WAI\tI:%u B%u\n", nes->mCPU.SSR.I, nes->mCPU.SSR.B);
        //     nes->mCPU.wait();
        //     break;

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
    {
        switch (e->type)
        {
            case SDL_EVENT_KEY_UP:
            case SDL_EVENT_KEY_DOWN:
                break;
            default:
                return;
        }

        auto &ctl = nes->mPlayerCtl[0];
        bool down = e->key.down;

        switch (e->key.key)
        {
            default: break;
            case SDLK_Z:     ctl.a     = down; break;
            case SDLK_X:     ctl.b     = down; break;
            case SDLK_TAB:   ctl.sel   = down; break;
            case SDLK_SPACE: ctl.start = down; break;
            case SDLK_UP:    ctl.up    = down; break;
            case SDLK_DOWN:  ctl.down  = down; break;
            case SDLK_LEFT:  ctl.left  = down; break;
            case SDLK_RIGHT: ctl.right = down; break;
        }
    }

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
            case SDL_GAMEPAD_BUTTON_SOUTH:      ctl.a     = down; break;
            case SDL_GAMEPAD_BUTTON_EAST:       ctl.b     = down; break;
            case SDL_GAMEPAD_BUTTON_BACK:       ctl.sel   = down; break;
            case SDL_GAMEPAD_BUTTON_START:      ctl.start = down; break;
            case SDL_GAMEPAD_BUTTON_DPAD_UP:    ctl.up    = down; break;
            case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  ctl.down  = down; break;
            case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  ctl.left  = down; break;
            case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: ctl.right = down; break;
        }
    }
}
