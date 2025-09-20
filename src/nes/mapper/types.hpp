#pragma once
#include <memu/nes/mapper.hpp>
#include <memu/addrspace.hpp>
#include <memu/nes/nes.hpp>

#define NESEMU_MAPPER_TYPES \
    X(Mapper000_NROM) \
    X(Mapper001_MMC1) \
    X(Mapper002_UxROM) \
    X(Mapper003_CNROM) \
    X(Mapper004_MMC3)


namespace NesEmu
{
    #define X(Nm) class Nm;
    NESEMU_MAPPER_TYPES
    #undef X
}


class NesEmu::Mapper000_NROM: public NesEmu::Mapper
{
private:
    iNES_File *mFile;

    struct PakBank
    {
        uint8_t *base;
        int32_t  size;
    };

    PakBank mPrgRom;
    PakBank mChrRom;

public:
    Mapper000_NROM(NesEmu::System&, GamePak*);

};



class NesEmu::Mapper001_MMC1: public NesEmu::Mapper
{
public:
    Mapper001_MMC1(NesEmu::System&, GamePak*) {  };
};



class NesEmu::Mapper002_UxROM: public NesEmu::Mapper
{
public:
    Mapper002_UxROM(NesEmu::System&, GamePak*) {  };
};



class NesEmu::Mapper003_CNROM: public NesEmu::Mapper
{
public:
    Mapper003_CNROM(NesEmu::System&, GamePak*) {  };
};



class NesEmu::Mapper004_MMC3: public NesEmu::Mapper
{
public:
    Mapper004_MMC3(NesEmu::System&, GamePak*) {  };
};

