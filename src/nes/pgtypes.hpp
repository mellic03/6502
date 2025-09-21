#pragma once

#include <memu/addrspace.hpp>
#include <memu/nes/cpu.hpp>
#include <memu/nes/ppu.hpp>


class PgEntryCpuPpu: public memu::iPageHandler
{
private:
    NesPPU &ppu;

public:
    PgEntryCpuPpu(NesPPU &p): ppu(p) {  }
    virtual ubyte read(addr_t addr) final;
    virtual void write(addr_t addr, ubyte data) final;

};



class PgEntryCpuIO: public memu::iPageHandler
{
private:
    NesCPU &cpu;

public:
    PgEntryCpuIO(NesCPU &c): cpu(c) {  }
    virtual ubyte read(addr_t addr) final;
    virtual void write(addr_t addr, ubyte data) final;

};

