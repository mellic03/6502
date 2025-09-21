#include <memu/display.hpp>
#include <iostream>


void EmuFramebuffer::blit( EmuFramebuffer *fb, ivec2 dstpos, float S )
{
    SDL_Rect src = { 0, 0, fb->mSp.x, fb->mSp.y };
    SDL_Rect dst = { dstpos.x, dstpos.y, int(S*fb->mSp.x), int(S*fb->mSp.y) };
    SDL_BlitSurfaceScaled(fb->mSurface, &src, mSurface, &dst, SDL_SCALEMODE_NEAREST);
}


void EmuFramebuffer::pixel( int x, int y, uint8_t *src )
{
    x %= mSp.x;
    y %= mSp.y;

    auto *info = SDL_GetPixelFormatDetails(mFormat);
    auto pitch = mSurface->pitch;
    auto bpp   = info->bytes_per_pixel; // ms->format->BytesPerPixel;

    uint8_t *dst = ((uint8_t*)mSurface->pixels) + pitch*y + bpp*x;

    memcpy(dst, src, 3);

    // dst[0] = src[0];
    // dst[1] = src[1];
    // dst[2] = src[2];

    // for (int i=0; i<3; i++)
    // {
    //     dst[i] = src[i];
    // }
}


void EmuFramebuffer::pixel( int x, int y, uint8_t value )
{
    ubyte buf[3];
    memset(buf, value, sizeof(buf));
    pixel(x, y, buf);
}


void EmuFramebuffer::pixel( int x, int y, uint8_t r, uint8_t g, uint8_t b )
{
    ubyte buf[3];
    buf[0] = r;
    buf[1] = g;
    buf[2] = b;
    pixel(x, y, buf);
}
