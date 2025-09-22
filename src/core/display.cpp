#include <memu/display.hpp>
#include <iostream>


void EmuFramebuffer::blit( EmuFramebuffer *fb, int dstx, int dsty, float S )
{
    SDL_Rect src = { 0, 0, fb->mSp.x, fb->mSp.y };
    SDL_Rect dst = { dstx, dsty, int(S*fb->mSp.x), int(S*fb->mSp.y) };
    SDL_BlitSurfaceScaled(fb->mSurface, &src, mSurface, &dst, SDL_SCALEMODE_NEAREST);
}

void EmuFramebuffer::pixel( int x, int y, uint8_t *src )
{
    x %= mSp.x;
    y %= mSp.y;

    auto *info = SDL_GetPixelFormatDetails(mFormat);
    auto pitch = mSurface->pitch;
    auto bpp   = info->bytes_per_pixel;

    uint8_t *dst = ((uint8_t*)mSurface->pixels) + pitch*y + bpp*x;

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
