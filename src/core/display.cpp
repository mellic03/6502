#include <memu/display.hpp>
#include <iostream>
#include <cstdio>



EmuImageBuffer::EmuImageBuffer( int w, int h, SDL_PixelFormat fmt )
:   EmuImageBuffer(SDL_CreateSurface(w, h, fmt))
{

}


EmuImageBuffer::EmuImageBuffer( SDL_Surface *S )
:   mSurface(S), mW(S->w), mH(S->h), mPitch(S->pitch),
    mBPP(SDL_GetPixelFormatDetails(S->format)->bits_per_pixel)
{
    SDL_assert((S != nullptr));
}


ubyte *EmuImageBuffer::getPixel( int x, int y )
{
    x %= mW;  y %= mH;
    return (ubyte*)(mSurface->pixels) + mPitch*y + mBPP*x;
}









EmuImageFont::EmuImageFont( const std::string &path )
:   EmuImageBuffer(SDL_LoadBMP(path.c_str()))
{

}


ivec2 EmuImageFont::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = mW / GLYPH_COLS;
    const int GLYPH_H = mH / GLYPH_ROWS;

    ivec2 sp = ivec2{GLYPH_W, GLYPH_H};
    // vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return sp;
}


ivec2 EmuImageFont::getGlyphCorner( char c )
{
    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=c && c<=cmax) == false)
    {
        return ivec2{-1, -1};
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = mW / GLYPH_COLS;
    const int GLYPH_H = mH / GLYPH_ROWS;

    int idx = int(c - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2{col*GLYPH_W, row*GLYPH_H};
}








void EmuFramebuffer::blit( EmuImageBuffer *buf, int x0, int y0 )
{
    SDL_Rect src = { 0, 0, buf->mW, buf->mH };
    SDL_Rect dst = { x0, y0, mW, mH };
    SDL_BlitSurfaceScaled(buf->mSurface, &src, mSurface, &dst, mScaleMode);
}


void EmuFramebuffer::blit( EmuImageBuffer *buf, int x0, int y0,
                           int x1, int y1, int w1, int h1 )
{
    SDL_Rect src = { x1, y1, w1, h1 };
    SDL_Rect dst = { x0, y0, mW, mH };
    SDL_BlitSurfaceScaled(buf->mSurface, &src, mSurface, &dst, mScaleMode);
}


void EmuFramebuffer::pixel( int x, int y, uint8_t *src )
{
    x %= mW;  y %= mH;
    ubyte *dst = (ubyte*)(mSurface->pixels) + mPitch*y + mBPP*x;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}


void EmuFramebuffer::pixel( int x, int y, ubyte r, ubyte g, ubyte b )
{
    ubyte buf[3] = {r, g, b};
    pixel(x, y, buf);
}












EmuWindow::EmuWindow( const char *title, int w, int h, int scale, int rate )
:   EmuFramebuffer(w, h),
    mWin(SDL_CreateWindow(title, scale*w, scale*h, 0)),
    mWinSurface(SDL_GetWindowSurface(mWin)),
    mScale(scale), mRate(rate), mTicks(rate), mFlushPending(false)
{
    SDL_SetWindowPosition(mWin, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

ubyte *EmuWindow::data()
{
    return static_cast<ubyte*>(mSurface->pixels);
}

void EmuWindow::_flush()
{
    SDL_Rect src = { 0, 0, mW, mH };
    SDL_Rect dst = { 0, 0, mScale*mW, mScale*mH };
    SDL_BlitSurfaceScaled(mSurface, &src, mWinSurface, &dst, mScaleMode);
    SDL_UpdateWindowSurface(mWin);
}

void EmuWindow::flush()
{
    mFlushPending = true;
}



EmuIO::EmuIO()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    mRunning = true;
}

void EmuIO::update()
{
    for (auto *win: mWindows)
    {
        SDL_assert((win->mSurface != nullptr));

        if (win->mFlushPending == true)
        {
            win->_flush();
            win->mFlushPending = false;
        }
        // if (++(win->mTicks) >= win->mRate)
        // {
        //     win->flush();
        //     win->mTicks = 0;
        // }
    }
}

void EmuIO::quit()
{
    SDL_Quit();
    mRunning = false;
}

EmuWindow *EmuIO::makeWin( const char *title, int w, int h, int s, int r )
{
    mWindows.push_back(new EmuWindow(title, w, h, s, r));
    return mWindows.back();
}

