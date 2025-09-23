#include <memu/font.hpp>

using namespace memu;



ivec2 ImageFont::getGlyphExtents()
{
    static constexpr int GLYPH_ROWS = 6;
    static constexpr int GLYPH_COLS = 16;
    const int GLYPH_W = mW / GLYPH_COLS;
    const int GLYPH_H = mH / GLYPH_ROWS;

    ivec2 sp = ivec2{GLYPH_W, GLYPH_H};
    // vec2 sp = glyph_scale * vec2(GLYPH_W, GLYPH_H);

    return sp;
}


ivec2 ImageFont::getGlyphCorner( char c )
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

