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
    void _entire_tile( int x0, int y0, uword tileIdx, uword palIdx );
    void _entire_frame();
    void _quik( int n );
    void _drawPattern( int dstx, int dsty, ubyte bgTile, ubyte row, ubyte col );
    void _drawNameTableRow( ubyte row );
    void _drawNameTableCell( ubyte row, ubyte col );

    union PtrnAddr {
        uword word;
        struct {
            uint8_t fineY     :3;
            uint8_t bitPlane  :1;
            uint8_t tileIdxLo :4;
            uint8_t tileIdxHi :4;
            uint8_t tableNo   :1;
            uint8_t zero      :3;
        } __attribute__((packed));

        //   PtrnAddr(uword w): word(0) {  }
        ubyte getIdx() { return (tileIdxHi << 4) | tileIdxLo; }
        // void setIdx(ubyte i) { tileIdxLo=(i & 0xFF); tileIdxHi=(i >> 8); }
    };


protected:
    ubyte *mFramebuffer;
    size_t mPitch;
    size_t mBPP;

public:
    EmuWindow *mWin;
    int mCycle;
    int mScanLine;
    int mPalNo = 0;

    // std::array<ubyte, 2*1024> mVRAM;
    uint8_t mVRAM[2048];
    uint8_t mPalette[192];
    uint8_t mPaletteCtl[32];


    Ricoh2C02(AddrSpace&, ubyte *fb, size_t pitch, size_t bpp);
    virtual void tick() override;
    virtual void reset() override;
    // int tickn( int n );
    void loadPalette(const std::string&);

};
