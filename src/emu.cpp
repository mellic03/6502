#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "6502/6502.hpp"
#include "nes/ppu.hpp"
#include "nes/ines.hpp"
#include "nes/mapper.hpp"


namespace emu { int entry(uint8_t*); }


#include <SDL2/SDL.h>

class Display
{
private:
public:
    SDL_Window  *mWin;
    SDL_Surface *mSurface;
    SDL_Surface *mSurfaceScaled;
    int mWidth;
    int mHeight;
    int mScale;

    void init( int w, int h, int scale )
    {
        mWidth  = w;
        mHeight = h;
        mScale  = scale;

        SDL_Init(SDL_INIT_VIDEO);

        mWin = SDL_CreateWindow(
            "6502 Emulation",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            scale*w,
            scale*h,
            0
        );

        mSurfaceScaled = SDL_GetWindowSurface(mWin);
        mSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    }

    void beginFrame()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if ((e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) ||
                (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) 
            {
                exit(0);
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
        
            }
        }

        SDL_PumpEvents();
    }

    void endFrame()
    {
        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = mWidth;
        src.h = mHeight;

        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        dest.w = mScale*mWidth;
        dest.h = mScale*mHeight;

        SDL_BlitScaled(mSurface, &src, mSurfaceScaled, &dest);

        SDL_UpdateWindowSurface(mWin);
    }


    void pixel( int x, int y )
    {
        x %= mWidth;
        y %= mHeight;

        Uint8 *const blue  = ((Uint8 *) mSurface->pixels + (y%mHeight)*4*mWidth + (x%mWidth)*4 + 0);
        *blue = 255;

        Uint8 *const green = ((Uint8 *) mSurface->pixels + (y%mHeight)*4*mWidth + (x%mWidth)*4 + 1);
        *green = 255;

        Uint8 *const red   = ((Uint8 *) mSurface->pixels + (y%mHeight)*4*mWidth + (x%mWidth)*4 + 2);
        *red = 255;
    }

};


// https://bugzmanov.github.io/nes_ebook/chapter_7.html

int emu::entry( uint8_t *rom )
{
    using BusAttachment6502 = cpu6502;

    // SignalEmitter hwtimer(1'790'000);
    SignalEmitter hwtimer(50);

    DataBus6502 cpu_bus;
    DataBusPPU  ppu_bus;

    auto cpu = BusAttachment6502(&cpu_bus);
    // cpu.LoadROM(rom);
    cpu.Listen(hwtimer);

    auto ppu = BusAttachmentPPU(&ppu_bus);
    ppu.Listen(hwtimer);



    // bool iNESFormat=false;
    // if (rom[0]=='N' && rom[1]=='E' && rom[2]=='S' && rom[3]==0x1A)
    //         iNESFormat=true;

    // bool NES20Format=false;
    // if (iNESFormat==true && (rom[7]&0x0C)==0x08)
    //         NES20Format=true;

    // printf("%d %d\n", iNESFormat, NES20Format);
    // return 0;

    INESHeader H = *(INESHeader*)rom;
    printf("signature   %s\n",     H.signature);
    printf("prgRomSize  %u\n", 16384 * H.prgRomSize);
    printf("chrRomSize  %u\n", 8192 * H.chrRomSize);
    printf("flags6      %u\n",     H.flags6);
    printf("flags7      %u\n",     H.flags7);
    printf("flags8      %u\n",     H.flags8);
    printf("flags9      %u\n",     H.flags9);
    printf("flags10     %u\n",     H.flags10);
    (MapperNROM()).MapROM(&cpu_bus, rom);
    cpu.PC = 0xC000;


    Display D;
    D.init(256, 240, 4);

    uint64_t tcurr = SDL_GetTicks64();
    uint64_t tprev = tcurr;


    while (!cpu.mInvalidOp)
    {
        D.beginFrame();

        tprev = tcurr;
        tcurr = SDL_GetTicks64();
        hwtimer.update(tcurr - tprev);

        // cpu.Tick();
        memcpy(D.mSurface->pixels, &(cpu.mBus[0]), 1024*32);

        D.endFrame();
    }
    return 0;
}


