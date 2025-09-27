#pragma once

#include <memu/types.hpp>
#include <SDL3/SDL.h>

#include <stdarg.h>
#include <string>
#include <vector>


class EmuImageBuffer;
class EmuImageFont;
class EmuFramebuffer;
class EmuWindow;
class EmuIO;


class EmuImageBuffer
{
public:
    // static constexpr SDL_PixelFormat mFormat    = SDL_PIXELFORMAT_RGB24;
    // static constexpr SDL_ScaleMode   mScaleMode = SDL_SCALEMODE_NEAREST;
    SDL_Surface *mSurface;
    int mW, mH, mPitch, mBPP;
    SDL_ScaleMode mScaleMode = SDL_SCALEMODE_NEAREST;

    EmuImageBuffer( int w, int h, SDL_PixelFormat fmt=SDL_PIXELFORMAT_RGB24 );
    EmuImageBuffer( SDL_Surface* );
    // EmuImageBuffer(): mSurface(0), mW(0), mH(0), mPitch(0), mBPP(0) {  };
    ubyte *getPixel( int x, int y );
};



class EmuImageFont: public EmuImageBuffer
{
private:
    // vec2 glyph_scale;

public:
    EmuImageFont( const std::string &filepath );
    // ImageFont( void *p, int w, int h, int bpp, int pitch )
    // :   mData((uint8_t*)p), mW(w), mH(h), mBpp(bpp), mPitch(pitch) {  };

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char ch );
};





class EmuFramebuffer: public EmuImageBuffer
{
public:
    using EmuImageBuffer::EmuImageBuffer;

    void blit( int x, int y, EmuImageBuffer *src );
    void blit( int x, int y, int w, int h, EmuImageBuffer *src, int sx, int sy, int sw, int sh );
    void pixel( int x, int y, uint8_t *src );
    void pixel( int x, int y, uint8_t r ) { pixel(x, y, r, r, r); };
    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b );
};



class EmuWindow
{
private:
    friend class EmuIO;

    struct glyph_t
    {
        EmuImageFont *f;
        char c;
        int x, y;
    };

    struct margin_t
    {
        int xmin, xmax, ymin, ymax;
    };

    std::vector<glyph_t> mGlyphs;

    ivec2          mGlyphPos;
    margin_t       mGlyphMargin;

    SDL_Window    *mWin;
    EmuFramebuffer mWinBuf;
    EmuFramebuffer mRealBuf;

    const int      mScale;
    const bool     mAutoUpdate;
    const size_t   mRate;
    int            mTicks;
    bool           mFlushPending;

    void _glyph( EmuImageFont*, char, int, int );
    void _flush();
    
public:
    void (*mOnUpdate)(EmuWindow*) = nullptr;
    EmuWindow( const char *title, int w, int h, int s=1, size_t r=1 );
    EmuFramebuffer *frameBuffer() { return &mRealBuf; }
    void glyph( EmuImageFont*, char ch, int x, int y );
    void str( EmuImageFont*, const char *str, int x, int y );
    void setMargin( int xmin, int xmax, int ymin, int ymax );
    void setScaleMode( SDL_ScaleMode mode ) { mRealBuf.mScaleMode = mode; }
    void setBounds( int x, int y, int w, int h );
    void print( EmuImageFont*, const char *fmt, ... );
    void flush() { mFlushPending = true; }
};



class EmuIO
{
private:
    std::vector<EmuWindow*> mWinAuto;
    std::vector<EmuWindow*> mWinExplicit;
    bool mRunning;

public:
    EmuIO();
    EmuWindow *makeWin( const char*, int w, int h, int scale, size_t rate=1 );
    bool running();
    void quit();
    void update();
};
