#pragma once

#include "bus.hpp"
#include "comm.hpp"
#include "clock.hpp"


struct ppc_regs_t
{
    uint32_t LR;
    uint32_t CTR;
    uint32_t XER;
    uint32_t FPSCR;

    uint32_t GPR[32];
    float    FPR[32];
};


struct ppc_cpu_t
{
    ppc_regs_t regs;
    emu::clock_device mClock;

    ppc_cpu_t();
};


// template <typename cpu_type>
struct ppc_board_t
{
    ppc_cpu_t *mCPU;
    emu::addr_device *mMainBus;

    ppc_board_t();
};

