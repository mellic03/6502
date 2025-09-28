#include <memu/display.hpp>
#include <iostream>
#include <cstdio>



EmuImageBuffer::EmuImageBuffer( int w, int h, SDL_PixelFormat fmt )
:   EmuImageBuffer(SDL_CreateSurface(w, h, fmt))
{

}


EmuImageBuffer::EmuImageBuffer( SDL_Surface *S )
:   mSurface(S), mW(S->w), mH(S->h), mPitch(S->pitch),
    mBPP(SDL_GetPixelFormatDetails(S->format)->bytes_per_pixel)
{
    SDL_assert((S != nullptr));
}


ubyte *EmuImageBuffer::getPixel( int x, int y )
{
    return (ubyte*)(mSurface->pixels) + mPitch*(y%mH) + mBPP*(x%mW);
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


ivec2 EmuImageFont::getGlyphCorner( char ch )
{
    static constexpr char cmin = ' ';
    static constexpr char cmax = '~';

    if ((cmin<=ch && ch<=cmax) == false)
    {
        return ivec2{-1, -1};
    }

    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;

    const int GLYPH_W = mW / GLYPH_COLS;
    const int GLYPH_H = mH / GLYPH_ROWS;

    int idx = int(ch - cmin);
    int row = idx / GLYPH_COLS;
    int col = idx % GLYPH_COLS;

    return ivec2{col*GLYPH_W, row*GLYPH_H};
}







void EmuFramebuffer::blit( int x, int y, EmuImageBuffer *sbuf )
{
    blit(x, y, mW, mH, sbuf, 0, 0, sbuf->mW, sbuf->mH);
    // SDL_Rect src = { 0, 0, buf->mW, buf->mH };
    // SDL_Rect dst = { x0, y0, mW, mH };
    // SDL_BlitSurfaceScaled(buf->mSurface, &src, mSurface, &dst, SDL_SCALEMODE_NEAREST);
}


void EmuFramebuffer::blit( int x, int y, int w, int h,
                           EmuImageBuffer *sbuf, int sx, int sy, int sw, int sh )
{
    SDL_Rect src = { sx, sy, sw, sh };
    SDL_Rect dst = { x, y, w, h };

    if (w==sw && h==sh)
    {
        SDL_BlitSurface(sbuf->mSurface, &src, mSurface, &dst);
    }

    else
    {
        SDL_BlitSurfaceScaled(sbuf->mSurface, &src, mSurface, &dst, mScaleMode);
    }
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




// :   EmuFramebuffer(SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGB24)),

EmuWindow::EmuWindow( const char *title, int w, int h, int scale, size_t rate )
:   mWin     (SDL_CreateWindow(title, scale*w, scale*h, 0)),
    mWinBuf  (SDL_GetWindowSurface(mWin)),
    mRealBuf (SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGB24)),
    mScale(scale), mAutoUpdate(mRate==-1), mRate(rate), mTicks(rate), mFlushPending(false)
{
    SDL_SetWindowPosition(mWin, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    mGlyphMargin = {0, w, 0, h};
}

void EmuWindow::_glyph( EmuImageFont *f, char c, int x, int y )
{
    ivec2 tl = f->getGlyphCorner(c);
    ivec2 sp = f->getGlyphExtents();
    SDL_Rect src = { tl.x, tl.y, sp.x, sp.y };
    SDL_Rect dst = { mScale*x, mScale*y, mScale*sp.x, mScale*sp.y };

    // if (s == 1)
    // {
    //     SDL_BlitSurface(f->mSurface, &src, mRealBuf.mSurface, &dst);
    // }

    // else
    // {
        SDL_BlitSurfaceScaled(f->mSurface, &src, mWinBuf.mSurface, &dst, SDL_SCALEMODE_LINEAR);
    // }
}

void EmuWindow::_flush()
{
    if (mOnUpdate)
    {
        mOnUpdate(this);
    }

    mWinBuf.blit(0, 0, mWinBuf.mW, mWinBuf.mH, &mRealBuf, 0, 0, mRealBuf.mW, mRealBuf.mH);

    mGlyphPos = {0, 0};
    for (auto &G: mGlyphs)
        _glyph(G.f, G.c, G.x, G.y);
    mGlyphs.clear();

    SDL_UpdateWindowSurface(mWin);
}

void EmuWindow::glyph( EmuImageFont *f, char c, int x, int y )
{
    mGlyphs.push_back({f, c, x, y});
}


void EmuWindow::str( EmuImageFont *f, const char *str, int x, int y )
{
    ivec2 sp = f->getGlyphExtents();

    while (char ch = *(str++))
    {
        if (ch == '\n')
            x = sp.x;

        else
            mGlyphs.push_back({f, ch, x, y});

        x += sp.x;
    
        if (x >= mWinBuf.mW)
        {
            x = 0;
            y += sp.y;
        }
    
        if (y >= mWinBuf.mH)
            return;
    }
}

void EmuWindow::setMargin( int xmin, int xmax, int ymin, int ymax )
{
    mGlyphMargin = { xmin, xmax, ymin, ymax };
    mGlyphPos    = { xmin, ymin };
}

void EmuWindow::setBounds( int x, int y, int w, int h )
{
    mGlyphMargin = { x, x+w, y, y+h };
    mGlyphPos    = { x, y };
}


void EmuWindow::print( EmuImageFont *f, const char *fmt, ... )
{
    static char buf[256];
    const char *str = &buf[0];

    va_list vlist;
    va_start(vlist, fmt);
    vsnprintf(buf, 256, fmt, vlist);
    va_end(vlist);

    ivec2 sp = f->getGlyphExtents();
    auto [xmin, xmax, ymin, ymax] = mGlyphMargin;
    auto &x = mGlyphPos.x;
    auto &y = mGlyphPos.y;

    if (x<xmin || x>xmax) x=xmin;
    if (y<ymin || y>ymax) y=ymin;

    while (char ch = *(str++))
    {
        if (ch == '\n')
            x = xmax+1;
        else
            mGlyphs.push_back({f, ch, x, y});

        x += (3*sp.x)/4;

        if (x >= xmax)
        {
            x = xmin;
            y += sp.y;
        }
    
        if (y >= ymax)
        {
            return;
        }
    }
}






EmuIO::EmuIO()
:   mRunning(true)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
}


EmuWindow *EmuIO::makeWin( const char *title, int w, int h, int scale, size_t rate )
{
    printf("makeWin\n");

    auto &winv = (rate==0) ? mWinExplicit : mWinAuto;
    winv.push_back(new EmuWindow(title, w, h, scale, rate));
    return winv.back();
}

bool EmuIO::running()
{
    return mRunning;
}

void EmuIO::quit()
{
    mRunning = false;
}

void EmuIO::update()
{
    for (auto *win: mWinAuto)
    {
        if (++(win->mTicks) >= win->mRate)
        {
            win->_flush();
            win->mTicks = 0;
        }
    }

    for (auto *win: mWinExplicit)
    {
        if (win->mFlushPending == true)
        {
            win->_flush();
            win->mFlushPending = false;
        }
    }

    // for (auto *win: mWinExplicit)
    //     win->_flush();

    // for (auto *win: mWinAuto)
    //     win->_flush();
}





