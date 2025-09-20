#pragma once

#include <SDL2/SDL.h>
#include <memu/types.hpp>
#include <vector>


class EmuFramebuffer
{
public:
    ivec2 mSp;
    SDL_Surface *mSurface;

    EmuFramebuffer( int w, int h )
    :   mSp{w, h}, mSurface(SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0))
    {
        
    }

    void blit( EmuFramebuffer *fb, ivec2 dstpos, float S=1.0f )
    {
        SDL_Rect src = { 0, 0, fb->mSp.x, fb->mSp.y };
        SDL_Rect dst = { dstpos.x, dstpos.y, int(S*fb->mSp.x), int(S*fb->mSp.y) };
        SDL_BlitScaled(fb->mSurface, &src, mSurface, &dst);
    }

    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b )
    {
        x %= mSp.x;
        y %= mSp.y;

        auto &ms   = mSurface;
        auto pitch = ms->pitch;
        auto bpp   = ms->format->BytesPerPixel;

        *((Uint8*)(ms->pixels) + (pitch*y + bpp*x) + 0) = b;
        *((Uint8*)(ms->pixels) + (pitch*y + bpp*x) + 1) = g;
        *((Uint8*)(ms->pixels) + (pitch*y + bpp*x) + 2) = r;
    }

    void pixel( int x, int y, uint8_t c=255 )
    {
        pixel(x, y, c, c, c);
    }
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

        mWin = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            scale*w,
            scale*h,
            0
        );

        mWinSurface = SDL_GetWindowSurface(mWin);
    }

    void flush()
    {
        SDL_Rect src = { 0, 0, mSp.x, mSp.y };
        SDL_Rect dst = { 0, 0, mScale*mSp.x, mScale*mSp.y };
        SDL_BlitScaled(mSurface, &src, mWinSurface, &dst);
        SDL_UpdateWindowSurface(mWin);
    }
};


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
        SDL_Init(SDL_INIT_VIDEO);
    }

    // EmuWindow *addWindow( EmuWindow *win )
    // {
    //     mWindows.push_back(win);
    //     return mWindows.back();
    // }

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
};
