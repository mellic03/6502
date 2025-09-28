#pragma once

#include <memu/hwmodule.hpp>
#include <memu/display.hpp>
#include <memu/memory.hpp>
#include <memu/latch.hpp>
#include "2C02_detail.hpp"
#include <string>
#include <array>
#include <vector>


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
    void _single_pixel( ubyte x, ubyte y );
    void _entire_tile( int x0, int y0, ubyte tileIdx, ubyte palIdx );
    void _entire_frame();

protected:

public:
    EmuWindow *mGameWin;

    int mScanLine;
    int mCycle;
    int mPalNo = 0;

    ubyte *ioLineCLK;
    ubyte *ioLineNMI;
    ubyte *ioLineRES;

    // std::array<ubyte, 2*1024> mVRAM;
    uint8_t mVRAM[2048];
    uint8_t mPalette[192];
    uint8_t mPaletteCtl[32];

    Ricoh2C02(AddrSpace&, EmuWindow *gamewin);
    virtual void tick() override;
    virtual void reset() override;

    void flush();
    void loadPalette(const std::string&);
    void preRenderChrTile( EmuWindow *fb, int x0, int y0, uword tsel, uword tidx, uword pidx );
    void preRenderChrRom( EmuWindow *fb );

};
