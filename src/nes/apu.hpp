#pragma once

#include "../hw/bus.hpp"
#include "../hw/tmemory.hpp"


enum class NesAPU_Reg: uint8_t
{
    PPUCTRL,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA,
    NumValues
};

enum class NesAPU_Rwx: uint8_t
{
    RWX_PPUCTRL   = RWX::W,
    RWX_PPUMASK   = RWX::W,
    RWX_PPUSTATUS = RWX::R,
    RWX_OAMADDR   = RWX::W,
    RWX_OAMDATA   = RWX::RW,
    RWX_PPUSCROLL = RWX::Wx2,
    RWX_PPUADDR   = RWX::Wx2,
    RWX_PPUDATA   = RWX::RW,
    NumValues     = 8
};


class NesAPU: public NesEmu::RegisterMMIO<NesAPU_Reg, NesAPU_Rwx>
{
private:
    using Reg = NesPPU_Reg;
    using Rwx = NesPPU_Rwx;

public:
    virtual void Tick() final {  };
};




