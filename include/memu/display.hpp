#pragma once

#include <SDL3/SDL.h>
#include <memu/types.hpp>
#include <vector>


class EmuFramebuffer
{
public:
    static constexpr SDL_PixelFormat mFormat    = SDL_PIXELFORMAT_RGB24;
    static constexpr SDL_ScaleMode   mScaleMode = SDL_SCALEMODE_NEAREST;

    ivec2 mSp;
    SDL_Surface *mSurface;

    EmuFramebuffer( int w, int h );
    void blit( EmuFramebuffer *fb, int dstx, int dsty );
    void pixel( int x, int y, uint8_t *src );
    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b );
    void rect( int x, int y, int w, int h, ubyte r, ubyte g, ubyte b );
};



class EmuWindow: public EmuFramebuffer
{
public:
    SDL_Window  *mWin;
    SDL_Surface *mWinSurface;
    int          mScale;

    EmuWindow( const char *title, int w, int h, int scale );
    void flush();
};



class EmuIO
{
public:
    bool  mRunning;
    // Uint8 mKeyCurr[512];
    // Uint8 mKeyPrev[512];

    EmuIO();

    void quit()
    {
        SDL_Quit();
        mRunning = false;
    }

private:


};
