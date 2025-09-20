#pragma once

#include <SDL2/SDL.h>
#include <memu/types.hpp>
#include <vector>


class EmuWindow
{
public:
    SDL_Window  *mWin;
    SDL_Surface *mSurface;
    SDL_Surface *mSurfaceScaled;
    ivec2        sp;
    int          mScale;

    EmuWindow( const char *title, int w, int h, int scale )
    {
        sp = { w, h };
        mScale = scale;

        mWin = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            scale*w,
            scale*h,
            0
        );

        mSurfaceScaled = SDL_GetWindowSurface(mWin);
        mSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    }

    void endFrame()
    {
        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = sp.x;
        src.h = sp.y;

        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        dest.w = mScale * sp.x;
        dest.h = mScale * sp.y;

        SDL_BlitScaled(mSurface, &src, mSurfaceScaled, &dest);

        SDL_UpdateWindowSurface(mWin);
    }

    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b )
    {
        x %= sp.x;
        y %= sp.y;

        auto &ms = mSurface;
        *((Uint8*)(ms->pixels) + (y*ms->pitch + x*ms->format->BytesPerPixel) + 0) = b;
        *((Uint8*)(ms->pixels) + (y*ms->pitch + x*ms->format->BytesPerPixel) + 1) = g;
        *((Uint8*)(ms->pixels) + (y*ms->pitch + x*ms->format->BytesPerPixel) + 2) = r;
    }

    void pixel( int x, int y, uint8_t value=255 )
    {
        pixel(x, y, value, value, value);
    }

};


class Display
{
private:
public:
    std::vector<EmuWindow*> mWindows;
    bool  mRunning;
    Uint8 mKeyCurr[512];
    Uint8 mKeyPrev[512];

    Display()
    {
        mRunning = true;
        memset(mKeyCurr, 0, 512);
        memset(mKeyPrev, 0, 512);
        SDL_Init(SDL_INIT_VIDEO);
    }

    EmuWindow *addWindow( EmuWindow *win )
    {
        mWindows.push_back(win);
        return mWindows.back();
    }

    bool keyReleased( int k )
    {
        return (mKeyPrev[k] == 1) && (mKeyCurr[k] == 0);
    }

    void beginFrame()
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

    void endFrame()
    {
        for (auto *win: mWindows)
        {
            win->endFrame();
        }
    }

};
