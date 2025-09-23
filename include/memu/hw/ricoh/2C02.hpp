#pragma once

#include <memu/hwmodule.hpp>
#include <memu/display.hpp>
#include <memu/memory.hpp>
#include <memu/latch.hpp>
#include "2C02_detail.hpp"
#include <string>
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

public:
    int16_t   mCycle;
    int16_t   mScanLine;
    int16_t   mPrevLine;

    DataLatch mPpuAddr;
    uint8_t   mPpuData;

    Memory2kRW mVRAM;
    uint8_t mPalette[192];
    uint8_t mPaletteCtl[32];


    Ricoh2C02(AddrSpace&);
    virtual void tick() = 0;
    virtual void reset() override;
    void loadPalette(const std::string&);


    uint8_t read2002()
    {
        printf("[read2002] vblank cleared\n");

        uint8_t result = ppustat.byte;
        ppustat.VBlank = 0;
        mPpuAddr.reset();
        return result;
    }

    uint8_t read2007()
    {
        ubyte  res  = mPpuData;
        uword &addr = mPpuAddr.value;

        mPpuData = rdbus(addr);
        if (addr >= 0x3F00)
            res = mPpuData;
        addr += (ppuctl.Increment) ? 32 : 1;
        addr &= 0x3FFF;

        return res;
    }


    void write2005( uint8_t data )
    {

    }

    void write2006( uint8_t data )
    {
        mPpuAddr.write(data);
    }

    void write2007( uint8_t data )
    {
        uword &addr = mPpuAddr.value;
        printf("[write2007] ppuAddr=%04X\n", addr);

        wtbus(addr, data);
        addr += (ppuctl.Increment) ? 32 : 1;
        addr &= 0x3FFF;
    }
};
