#pragma once

#include "types.hpp"
#include <string>


struct SDL_Surface;

namespace memu
{
    struct ImageFont;
}


struct memu::ImageFont
{
private:
    
    uint8_t *mData;
    int      mW;
    int      mH;
    int      mBpp;
    int      mPitch;
    // vec2    glyph_scale;
public:
    ImageFont( const std::string &filepath );
    // ImageFont( void *p, int w, int h, int bpp, int pitch )
    // :   mData((uint8_t*)p), mW(w), mH(h), mBpp(bpp), mPitch(pitch) {  };

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char c );
};