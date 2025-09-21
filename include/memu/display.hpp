#pragma once

#include <SDL3/SDL.h>
#include <memu/types.hpp>
#include <vector>


class EmuFramebuffer
{
public:
    static constexpr SDL_PixelFormat mFormat = SDL_PIXELFORMAT_RGB24;

    ivec2 mSp;
    SDL_Surface *mSurface;

    EmuFramebuffer( int w, int h )
    :   mSp{w, h}, mSurface(SDL_CreateSurface(w, h, mFormat)) {  }

    void blit( EmuFramebuffer *fb, ivec2 dstpos, float S=1.0f );
    void pixel( int x, int y, uint8_t *src );
    void pixel( int x, int y, uint8_t value=255 );
    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b );
};


class EmuWindow: public EmuFramebuffer
{
public:
    SDL_Window  *mWin;
    SDL_Surface *mWinSurface;
    int          mScale;

    EmuWindow( const char *title, int w, int h, int scale )
    :   EmuFramebuffer(w, h)
    {
        mSp    = { w, h };
        mScale = scale;
        mWin = SDL_CreateWindow(title, scale*w, scale*h, 0);
        mWinSurface = SDL_GetWindowSurface(mWin);
    }

    void flush()
    {
        SDL_Rect src = { 0, 0, mSp.x, mSp.y };
        SDL_Rect dst = { 0, 0, mScale*mSp.x, mScale*mSp.y };
        SDL_BlitSurfaceScaled(mSurface, &src, mWinSurface, &dst, SDL_SCALEMODE_NEAREST);
        SDL_UpdateWindowSurface(mWin);
    }
};

#include <stdio.h>

class EmuIO
{
private:
public:
    bool  mRunning;
    Uint8 mKeyCurr[512];
    Uint8 mKeyPrev[512];

    EmuIO()
    {
        mRunning = true;
        memset(mKeyCurr, 0, 512);
        memset(mKeyPrev, 0, 512);
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    }

    bool keyReleased( int k )
    {
        return (mKeyPrev[k] == 1) && (mKeyCurr[k] == 0);
    }

    void updateEvents()
    {
        int numkeys = 0;
        auto *state = SDL_GetKeyboardState(&numkeys);
        memcpy(mKeyPrev, mKeyCurr, numkeys);
        memcpy(mKeyCurr, state, numkeys);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_EVENT_QUIT:
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    SDL_Quit();
                    break;

                default:
                    break;
            }
        }

        SDL_PumpEvents();
    }
};
