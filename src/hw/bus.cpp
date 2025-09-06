#include "bus.hpp"
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cstdlib>


#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "(%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }

static uint16_t dummy_dc( uint16_t i ) { return i; }

static uint8_t dummy_rd( uint16_t )
{ fprintf(stderr, "[DataBus::DataBus] Bad read\n"); exit(1); return 0; }

static void dummy_wt( uint16_t, uint8_t )
{ fprintf(stderr, "[DataBus::DataBus] Bad write\n"); exit(1); }


DataBus::DataBus()
{
    memset(mPageTable, 0, sizeof(mPageTable));
    mPageFuncs.push_back({nullptr, nullptr, nullptr, nullptr});
}

void DataBus::tick()
{
    for (iBusDevice *dev: mDevices)
    {
        dev->Tick();
    }
}


void DataBus::attach( iBusDevice *dev )
{
    dev->mBus = this;
    mDevices.insert(dev);
}


void DataBus::mapPage( uint16_t addr, uint8_t *page )
{
    mPageTable[addr/256] = page;
}


uint8_t DataBus::read( uint16_t addr )
{
    // uint8_t *page = mPageTable[addr >> 8];
    return mPageTable[addr >> 8][addr & 0x00FF];

    // auto &[dev, dc, rd, wt] = mPageFuncs[mPageTable[addr/256]];
    // addr = dc(addr);

    // if (!dev || !rd)
    // {
    //     fprintf(stderr, "[DataBus::read] Bad read *0x%04X\n", addr);
    //     fprintf(stderr, "(%s:%d)\n\n", __FILE__, __LINE__);
    //     exit(1);
    // }
    // return rd(dev, addr);
}

void DataBus::write( uint16_t addr, uint8_t byte )
{
    auto &[dev, dc, rd, wt] = mPageFuncs[mPageTable[addr/256]];
    addr = dc(addr);

    if (!dev || !wt)
    {
        fprintf(stderr, "[DataBus::write] Bad write *0x%04X = 0x%02X\n", addr, byte);
        fprintf(stderr, "(%s:%d)\n\n", __FILE__, __LINE__);
        exit(1);
    }
    wt(dev, addr, byte);
}
