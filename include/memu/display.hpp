#pragma once

#include <SDL3/SDL.h>
#include <memu/types.hpp>
#include <string>
#include <vector>



class EmuImageBuffer
{
protected:
    EmuImageBuffer( SDL_Surface*, int w, int h, ubyte *pxl, int pitch, int bpp );

public:
    static constexpr SDL_PixelFormat mFormat    = SDL_PIXELFORMAT_RGB24;
    static constexpr SDL_ScaleMode   mScaleMode = SDL_SCALEMODE_NEAREST;

    SDL_Surface *mSurface;
    int    mW;
    int    mH;
    size_t mPitch;
    size_t mBPP;

    EmuImageBuffer( int w, int h, SDL_PixelFormat fmt=SDL_PIXELFORMAT_RGB24 );
    EmuImageBuffer( SDL_Surface* );
    EmuImageBuffer(): mSurface(0), mW(0), mH(0), mPitch(0), mBPP(0) {  }
    ubyte *getPixel( int x, int y );
};



struct EmuImageFont: public EmuImageBuffer
{
private:
    // vec2 glyph_scale;

public:
    EmuImageFont( const std::string &filepath );
    // ImageFont( void *p, int w, int h, int bpp, int pitch )
    // :   mData((uint8_t*)p), mW(w), mH(h), mBpp(bpp), mPitch(pitch) {  };

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char c );
};






class EmuFramebuffer: public EmuImageBuffer
{
public:
    using EmuImageBuffer::EmuImageBuffer;

    void blit( EmuImageBuffer *buf, int x0, int y0 );
    void blit( EmuImageBuffer *buf, int x0, int y0, int x1, int y1, int w1, int h1 );
    void pixel( int x, int y, uint8_t *src );
    void pixel( int x, int y, uint8_t r ) { pixel(x, y, r, r, r); };
    void pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b );
};




class EmuWindow: public EmuFramebuffer
{
private:
    friend class EmuIO;

    SDL_Window  *mWin;
    SDL_Surface *mWinSurface;
    const int    mScale;
    const int    mRate;
    int          mTicks;
    bool         mFlushPending;

    EmuWindow( const char *title, int w, int h, int s=1, int r=1 );
    void _flush();
    
public:
    ubyte *data();
    void  flush();
};


class EmuIO
{
public:
    bool mRunning;
    // Uint8 mKeyCurr[512];
    // Uint8 mKeyPrev[512];

    EmuIO();
    void update();
    void quit();
    EmuWindow *makeWin( const char *title, int w, int h, int scale, int rate=1 );

private:
    std::vector<EmuWindow*> mWindows;

};
