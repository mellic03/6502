#pragma once

#include <memu/hwmodule.hpp>
#include <memu/memory.hpp>
#include "2C02_detail.hpp"
#include <string>


namespace memu
{
    class Ricoh2C02;
}

class memu::Ricoh2C02: public memu::HwModule, public RP2C02_detail::BaseHw
{
public:
    uint8_t mPalette[192];
    uint8_t mPaletteIndices[32];

    /**
     * - Tile pixel has four possible values: 00, 01, 10, 11.
     * --> tile[r, c] == (msb[r, c] << 1) | lsb[r, c]
     * - Tile is 8x8 cells, each row is one byte.
     * --> Tile uses 16 bytes of memory: 8 msb rows + 8 lsb rows.
     */
    struct PatternTable {
        ubyte lsb[8];
        ubyte msb[8];
    } __attribute__((packed));

    Memory2kRW mVRAM;

    Ricoh2C02(AddrSpace &bus)
    :   HwModule(bus), BaseHw(), mAccum(0), mScanline(0) {  }
    virtual size_t tick() override;
    virtual void reset() override;
    void loadPalette(const std::string&);

private:
    size_t mAccum;
    size_t mScanline;

};
