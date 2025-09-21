#pragma once

#include <memu/addrspace.hpp>
#include <memu/nes/cpu.hpp>
#include <memu/nes/ppu.hpp>


class PgEntryCpuPpu: public memu::iPageHandler
{
private:
    NesPPU &mPPU;

public:
    PgEntryCpuPpu(NesPPU &ppu): mPPU(ppu) {  }
    virtual ubyte read(addr_t addr) final;
    virtual void write(addr_t addr, ubyte data) final;

};



class PgEntryCpuIO: public memu::iPageHandler
{
private:
    NesCPU &mCPU;

public:
    PgEntryCpuIO(NesCPU &cpu): mCPU(cpu) {  }
    virtual ubyte read(addr_t addr) final;
    virtual void write(addr_t addr, ubyte data) final;

};

