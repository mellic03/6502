#pragma once

#include "types.hpp"


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
    ImageFont( void *p, int w, int h, int bpp, int pitch )
    :   mData((uint8_t*)p), mW(w), mH(h), mBpp(bpp), mPitch(pitch) {  };

    ivec2 getGlyphExtents();
    ivec2 getGlyphCorner( char c );
};