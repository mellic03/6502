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
    class Mapper000_NROM;
    class Mapper001_MMC1;
    class Mapper002_UxROM;
    class Mapper003_CNROM;
    class Mapper004_MMC3;
    // #define X(Nm) class Nm;
    // NESEMU_MAPPER_TYPES
    // #undef X
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

    PakBank mPrgRam;
    PakBank mPrgRom;
    PakBank mChrRom;


    class CpuAccess: public memu::iPageHandler
    {
    public:
        CpuAccess(System &n, Mapper000_NROM &m)
        :   mAddrLatch(true), nes(n), nrom(m) {  }
        virtual ubyte read(addr_t) final;
        virtual void write(addr_t, ubyte) final;

    private:
        bool mAddrLatch;
        NesEmu::System &nes;
        Mapper000_NROM &nrom;
        ubyte read_ppu(addr_t);
        void write_ppu(addr_t, ubyte);
    };

    class PpuAccess: public memu::iPageHandler
    {
    private:
        NesEmu::System &nes;
        Mapper000_NROM &nrom;
    public:
        PpuAccess(System &n, Mapper000_NROM &m): nes(n), nrom(m) {  }
        virtual ubyte read(addr_t) final;
        virtual void write(addr_t, ubyte) final;
    };

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

