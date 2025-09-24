#include <memu/display.hpp>
#include <iostream>
#include <cstdio>



EmuFramebuffer::EmuFramebuffer( int w, int h )
:   mSurface(SDL_CreateSurface(w, h, mFormat)),
    mW(w), mH(h)
{
    mData  = mSurface->pixels;
    mPitch = mSurface->pitch;
    mBPP   = SDL_GetPixelFormatDetails(mFormat)->bytes_per_pixel;
    mSp    = { w, h };
}


void EmuFramebuffer::blit( EmuFramebuffer *fb, int dstx, int dsty )
{
    SDL_Rect src = { 0, 0, fb->mSp.x, fb->mSp.y };
    SDL_Rect dst = { dstx, dsty, mSp.x, mSp.y };
    SDL_BlitSurfaceScaled(fb->mSurface, &src, mSurface, &dst, mScaleMode);
}


void EmuFramebuffer::pixel( int x, int y, uint8_t *src )
{
    x %= mSp.x;
    y %= mSp.y;

    ubyte *dst = (ubyte*)mData + mPitch*y + mBPP*x;

    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}


void EmuFramebuffer::pixel( int x, int y, ubyte r, ubyte g, ubyte b )
{
    ubyte buf[3] = {r, g, b};
    pixel(x, y, buf);
}


void EmuFramebuffer::rect( int tlx, int tly, int w, int h, ubyte r, ubyte g, ubyte b )
{
    for (int y=tly; y<tly+h; y++)
    {
        for (int x=tlx; x<tlx+w; x++)
        {
            pixel(x, y, r, g, b);
        }
    }
}




EmuWindow::EmuWindow( const char *title, int w, int h, int s )
:   EmuFramebuffer(w, h),
    mWin(SDL_CreateWindow(title, s*w, s*h, 0)),
    mWinSurface(SDL_GetWindowSurface(mWin)),
    mScale(s)
{
    // mWin = SDL_CreateWindow(title, scale*w, scale*h, 0);
    SDL_SetWindowPosition(mWin, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

ubyte *EmuWindow::data()
{
    return static_cast<ubyte*>(mSurface->pixels);
}

void EmuWindow::flush()
{
    SDL_Rect src = { 0, 0, mSp.x, mSp.y };
    SDL_Rect dst = { 0, 0, mScale*mSp.x, mScale*mSp.y };
    SDL_BlitSurfaceScaled(mSurface, &src, mWinSurface, &dst, mScaleMode);
    SDL_UpdateWindowSurface(mWin);
}




EmuIO::EmuIO()
{
    mRunning = true;
    // memset(mKeyCurr, 0, 512);
    // memset(mKeyPrev, 0, 512);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

}

// bool EmuIO::keyReleased( int k )
// {
//     return (mKeyPrev[k] == 1) && (mKeyCurr[k] == 0);
// }

// void EmuIO::updateEvents()
// {
//     int numkeys = 0;
//     auto *state = SDL_GetKeyboardState(&numkeys);
//     memcpy(mKeyPrev, mKeyCurr, numkeys);
//     memcpy(mKeyCurr, state, numkeys);

//     SDL_Event e;
//     while (SDL_PollEvent(&e))
//     {
//         switch (e.type)
//         {
//             default: break;
//             case SDL_EVENT_QUIT:
//             case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
//                 _quit();
//                 break;
//         }
//     }

//     SDL_PumpEvents();
// }

