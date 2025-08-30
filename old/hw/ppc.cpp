#include "hw.hpp"
#include "ppc.hpp"
#include "comm.hpp"



static void ppc_tick( void *arg )
{
    auto *board = static_cast<ppc_board_t*>(arg);
    // board->mMainBus->broadcast()
}


ppc_cpu_t::ppc_cpu_t()
:   mClock(ppc_tick)
{

}


ppc_board_t::ppc_board_t()
{
    mCPU      = emu::HwNew<ppc_cpu_t>();
    mMainBus  = emu::HwNew<emu::addr_device>(emu::BUSW_32);
    HwTestDevice1 *tdev1 = emu::HwNew<HwTestDevice1>();
    HwTestDevice2 *tdev2 = emu::HwNew<HwTestDevice2>();
    // auto *rom = emu::HwNew<HwBusROM<512>>();
    mMainBus->attach(tdev1);
    mMainBus->attach(tdev2);

}


void rerer()
{
    ppc_board_t board;
    board.mCPU->mClock.Tick(&board);
}
