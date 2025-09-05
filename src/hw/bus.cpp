#include "bus.hpp"
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cstdlib>


#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "(%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }

DataBus::DataBus()
{
    memset(mPageTable, 0, sizeof(mPageTable));

    mPageFuncs.push_back({
        [](uint16_t){
            fprintf(stderr, "[DataBus::DataBus] Bad read\n");
            exit(1); return 0;
        },
        [](uint16_t, uint8_t){
            fprintf(stderr, "[DataBus::DataBus] Bad write\n");
            exit(1); return;
        }
    });
}

void DataBus::tick()
{
    for (iBusDevice *dev: mDevices)
    {
        dev->Tick();
    }
}

void DataBus::map( uint16_t min, uint16_t max, ReadFunc rd, WriteFunc wt )
{
    uint16_t idx = mPageFuncs.size();
    mPageFuncs.push_back({rd, wt});

    for (uint32_t i=min; i<=max; i++)
    {
        mPageTable[i] = idx;
    }

    // uint16_t minPage = min/256;
    // uint16_t maxPage = max/256;

    // for (uint16_t page=minPage; page<=maxPage; page++)
    // {
    //     mPageTable[page] = idx;
    // }
}

void DataBus::attach( iBusDevice *dev )
{
    mDevices.insert(dev);
}

uint8_t DataBus::read( uint16_t addr )
{
    return mPageFuncs[mPageTable[addr/256]].read(addr);
}

void DataBus::write( uint16_t addr, uint8_t byte )
{
    mPageFuncs[mPageTable[addr/256]].write(addr, byte);
}
