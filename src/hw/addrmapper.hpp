#pragma once

#include "addrspace.hpp"
#include <vector>


namespace Emu
{
    struct Bank;
    struct HwMapper;
}


struct Emu::Bank
{
    uint8_t *page;
    uint16_t base;
    uint16_t mask;

    // Bank( uint8_t *pge, uint16_t bse, uint16_t msk )
    // :   page(pge), base(bse), mask(msk) {  };

    ubyte read( uint16_t ) const;
    void write( uint16_t, uint8_t );
};

// ubyte Emu::Bank::read( uint16_t i ) const
// {
//     return page[(i - base) & mask];
// }

// void Emu::Bank::write( uint16_t i, uint8_t v )
// {
//     page[(i - base) & mask] = v;
// }


struct Emu::HwMapper
{
    std::vector<uint8_t> &mPrgROM;
    std::vector<uint8_t> &mChrROM;

    Bank mPrgLo, mPrgHi;
    Bank mChrLo, mChrHi;

    HwMapper( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    :   mPrgROM(prgROM), mChrROM(chrROM) {  };
};



struct HwMapperNROM final: public Emu::HwMapper
{
    HwMapperNROM( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    :   HwMapper(prgROM, chrROM) {  };
};


