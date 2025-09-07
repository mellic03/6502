#pragma once

#include "../hw/addrspace.hpp"
#include "../hw/hwmapper.hpp"
#include "file/ines.hpp"
#include <vector>


namespace NesEmu
{
    struct Bank;
    class Mapper;
    Mapper *CreateMapper( int mapno, uint8_t *fileptr ); 
}


struct NesEmu::Mapper: public Emu::HwMapper
{
protected:
    struct iNES_File {
        iNES_Header header;
        uint8_t     data[];
    } *mFile;

public:
    Mapper( uint8_t *fileptr );
    
};


class MapperNROM final: public NesEmu::Mapper
{
private:
    // std::vector<uint8_t> &mPrgROM;
    // std::vector<uint8_t> &mChrROM;
    struct {
        Emu::Page prgLo, prgHi;
        Emu::Page chrLo, chrHi;
    } mBanks;

public:
    using Mapper::Mapper;
    // MapperNROM( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    // :   mPrgROM(prgROM), mChrROM(chrROM) {  };
};


class MapperMMC1 final: public NesEmu::Mapper
{
private:
    // std::vector<uint8_t> &mPrgROM;
    // std::vector<uint8_t> &mChrROM;
public:
    using Mapper::Mapper;
    // MapperMMC1( std::vector<uint8_t> &prgROM, std::vector<uint8_t> &chrROM )
    // :   mPrgROM(prgROM), mChrROM(chrROM) {  };
};


class MapperUxROM final: public NesEmu::Mapper
{
public:
    using Mapper::Mapper;

};


class MapperCNROM final: public NesEmu::Mapper
{
public:
    using Mapper::Mapper;

};


class MapperMMC3 final: public NesEmu::Mapper
{
public:
    using Mapper::Mapper;

};
