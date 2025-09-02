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
// #include "nes/nes.hpp"


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
    bool mRunning;

    void init( int w, int h, int scale )
    {
        mWidth  = w;
        mHeight = h;
        mScale  = scale;
        mRunning = true;

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



static uint8_t *cpureadpram(iBusDevice *dev, uint16_t base, uint16_t addr)
{
    addr = base + (addr % 0x4000);
    return ((MemoryDevice*)dev)->mMem + (addr - base);
}

static void cpuwritepram(iBusDevice *dev, uint16_t base, uint16_t addr, uint8_t byte)
{
    addr = base + (addr % 0x4000);
    (*(MemoryDevice*)dev)[addr-base] = byte;
}


static uint8_t *cpureadvram( iBusDevice *dev, uint16_t base, uint16_t addr )
{
    addr = 0x0200 + (addr % 8);
    return ((MemoryDevice*)dev)->mMem + (addr - base);
}

static void cpuwritevram( iBusDevice *dev, uint16_t base, uint16_t addr, uint8_t byte )
{
    addr = 0x0200 + (addr % 8);
    (*(MemoryDevice*)dev)[addr-0x2000] = byte;
}


int emu::entry( uint8_t *rom )
{
    using BusDevice6502 = cpu6502;

    SignalEmitter hwtimer(1'790'000);
    BusDevice6502 cpu;  cpu.Listen(hwtimer);
    BusDevicePPU  ppu;  ppu.Listen(hwtimer);

    cpu.mBus.attach(&(ppu.mRam), 0x2000, 0x3FFF, cpureadvram, cpuwritevram);

    // void MapROM( ... )
    {
        INESHeader H = *(INESHeader*)rom;
        uint16_t prgsize = 16384 * H.prgRomSize;
        uint16_t chrsize = 8192  * H.chrRomSize;

        uint8_t mapper = (0b11110000 & H.flags6) >> 4
                       + (0b11110000 & H.flags7) >> 4;

        bool iNESFormat  = false;
        bool NES20Format = false;

        if (rom[0]=='N' && rom[1]=='E' && rom[2]=='S' && rom[3]==0x1A)
        {
            if (iNESFormat && (rom[7]&0x0C)==0x08)
                NES20Format = true;
            else
                iNESFormat = true;
        }

        printf("signature   %s\n", H.signature);
        printf("prgRomSize  %u\n", prgsize);
        printf("chrRomSize  %u\n", chrsize);
        printf("mapper      %u\n", mapper);
        printf("flags6      %u\n", H.flags6);
        printf("flags7      %u\n", H.flags7);
        printf("flags8      %u\n", H.flags8);
        printf("flags9      %u\n", H.flags9);
        printf("flags10     %u\n", H.flags10);
        printf("\n");

        if (iNESFormat)
        {
            /*
                - All Banks are fixed,
                - CPU $6000-$7FFF:
                    Family Basic only: PRG RAM, mirrored as necessary to fill
                    entire 8 KiB window, write protectable with an external switch
                - CPU $8000-$BFFF:
                    First 16 KB of ROM.
                - CPU $C000-$FFFF:
                    Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
            */

            auto *prgRom = new MemoryDevice(prgsize);
            memcpy(prgRom->mMem, rom+0x10, prgsize);
            cpu.mBus.attach(prgRom, 0x8000, 0xFFFF, cpureadpram, cpuwritepram);
            // cpu.mBus.attach(&(cpu.mRam), 0x6000, 0x7FFF, cpureadram, cpuwriteram);


            auto *chrRom = new MemoryDevice(chrsize);
            memcpy(chrRom->mMem, rom+prgsize, chrsize);

            // memcpy(bus->mMem + 0x8000, rom+0x10, prgsize);

            // memcpy(cpubus->mMem + 0x4000, rom+0x10, prgsize);
            // memcpy(ppubus->mMem + 0x4000, rom+0x10, prgsize);
            // printf("BOYE: 0x%04X\n", bus->mMem[0x6000 + 16834 - 2]);
        }
        
        if (NES20Format)
        {
            
        }
    }




    // (MapperNROM()).MapROM(&bus6502, rom);
    cpu.PC = 0xC000;
    printf("WOOP  %02X %02X\n", cpu.mBus[0xFFFA], cpu.mBus[0xFFFB]);
    printf("WOOP  %02X %02X\n", cpu.mBus[0xFFFC], cpu.mBus[0xFFFD]);
    printf("WOOP  %02X %02X\n", cpu.mBus[0xFFFE], cpu.mBus[0xFFFF]);
    // cpu.PC = cpu.mBus.read16(0xFFFC);
    // return 0;

    Display D;
    D.init(256, 240, 4);

    uint64_t tcurr = SDL_GetTicks64();
    uint64_t tprev = tcurr;

    // for (int i=0; i<64&&!cpu.mInvalidOp; i++)
    while (!cpu.mInvalidOp)
    {
        D.beginFrame();

        tprev = tcurr;
        tcurr = SDL_GetTicks64();
        hwtimer.update(tcurr - tprev);

        D.endFrame();

        if (cpu.mCycles >= 400)
        {
            break;
        }
    }
    return 0;
}


