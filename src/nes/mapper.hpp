#pragma once

#include "../hw/addrspace.hpp"
#include "../hw/hwmapper.hpp"
#include <vector>

namespace NesEmu
{
    struct Bank;
    struct MapperNROM;  // 00
    struct MapperMMC1;  // 01
    struct MapperUxROM; // 02
    struct MapperCNROM; // 03
    struct MapperMMC3;  // 04
}

// struct NesEmu::Bank
// {
//     uint8_t *page;
//     uint16_t base;
//     uint16_t mask;

//     Bank( uint8_t *pge, uint16_t bse, uint16_t msk )
//     :   page(pge), base(bse), mask(msk) {  };

//     ubyte read( uint16_t i ) const
//     {
//         return page[(i - base) & mask];
//     }

//     void write( uint16_t i, uint8_t v )
//     {
//         page[(i - base) & mask] = v;
//     }
// };


struct NesEmu::MapperNROM final: public Emu::HwMapper
{
    std::vector<uint8_t> &mPrgROM;
    std::vector<uint8_t> &mChrROM;

    struct {
        Emu::Page prgLo, prgHi;
        Emu::Page chrLo, chrHi;
    } mBanks;

    MapperNROM( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    :   mPrgROM(prgROM), mChrROM(chrROM) {  };
};


struct NesEmu::MapperMMC1 final: public Emu::HwMapper
{
    MapperMMC1( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    :   HwMapper(prgROM, chrROM) {  };
};


