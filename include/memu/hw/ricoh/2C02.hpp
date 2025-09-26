#pragma once

#include <memu/hwmodule.hpp>
#include <memu/display.hpp>
#include <memu/memory.hpp>
#include <memu/latch.hpp>
#include "2C02_detail.hpp"
#include <string>
#include <array>
#include <vector>

struct uvec3 {
    uint8_t r, g, b;
} __attribute__((packed));


namespace memu
{
    class Ricoh2C02;
}


class memu::Ricoh2C02: public memu::HwModule, public RP2C02_detail::BaseHw
{
private:
    void _quik( int n );
    void _drawPattern( int dstx, int dsty, ubyte bgTile, ubyte row, ubyte col );
    void _drawNameTableRow( ubyte row );
    void _drawNameTableCell( ubyte row, ubyte col );

protected:

public:
    void preRenderChrTile( EmuFramebuffer *fb, int x0, int y0, uword tileIdx, uword palIdx );
    void preRenderChrRom( EmuFramebuffer *fb );

    void _entire_tile( int x0, int y0, uword tileIdx, uword palIdx );
    void _entire_frame();

    EmuWindow *mGameWin;
    EmuFramebuffer *mChrWin;

    int mCycle;
    int mScanLine;
    int mPalNo = 0;

    // std::array<ubyte, 2*1024> mVRAM;
    uint8_t mVRAM[2048];
    uint8_t mPalette[192];
    uint8_t mPaletteCtl[32];


    Ricoh2C02(AddrSpace&, EmuWindow *gamewin, EmuFramebuffer *chrwin);
    virtual void tick() override;
    virtual void reset() override;
    void loadPalette(const std::string&);

};
