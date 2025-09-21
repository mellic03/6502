#pragma once

#include <memu/hwmodule.hpp>
#include <memu/memory.hpp>
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
    size_t mAccum;
    size_t mScanline;

public:
    // uvec3   mPalette[64];
    uint8_t mPalette[192];
    uint8_t mPaletteCtl[32];

    Memory2kRW mVRAM;
    bool mAddrLatch = false;

    Ricoh2C02(AddrSpace&);
    virtual size_t tick() override;
    virtual void reset() override;
    void loadPalette(const std::string&);


    uint8_t read2002()
    {
        STATUS.V = 1;
        uint8_t result = STATUS.byte;
        STATUS.V = 0;          // clear vblank flag
        mAddrLatch = false;    // reset $2005/$2006 latch
        return result;
    }

    uint8_t read2007()
    {
        uint8_t result = mPpuData;
        mPpuData = rdbus(mPpuAddr);
        if (mPpuAddr >= 0x3F00)
            result = mPpuData;
        mPpuAddr += (CTRL.I) ? 32 : 1;
        mPpuAddr &= 0x3FFF;
        return result;
    }


    void write2005( uint8_t data )
    {

    }

    void write2006( uint8_t data )
    {
        if (!mAddrLatch) mPpuAddrHi = data;
        if ( mAddrLatch) mPpuAddrLo = data;
        mAddrLatch = !mAddrLatch;
    }

    void write2007( uint8_t data )
    {
        printf("[write2007] mPpuAddr=%04X\n", mPpuAddr);
        wtbus(mPpuAddr, data);
        mPpuAddr += (CTRL.I) ? 32 : 1;
        mPpuAddr &= 0x3FFF;
    }
};
