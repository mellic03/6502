#pragma once

#include <stddef.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <functional>

#include "device.hpp"
#include "../rwx.hpp"

class HwDevice;


class DataBus
{
private:
    struct PageTableEntry
    {
        uint8_t *page;
        uint16_t base;
        uint16_t mask;
        uint8_t  rwx;
        uint8_t  in_use;
    
        PageTableEntry()
        :   page(nullptr), base(0x0000), mask(0x0000), rwx(0), in_use(10) {  }

        PageTableEntry( uint8_t b, uint8_t m, uint8_t r, uint8_t *p )
        :   page(p), base(b), mask(m), rwx(r), in_use(1) {  }
    };

    std::set<HwDevice*> mDevices;
    PageTableEntry      mPageTables[256];

public:
    DataBus();

    void tick( uint64_t dt );
    void attach( HwDevice* );

    const uint8_t *getReadPtr( uint16_t addr );
    uint8_t *getWritePtr( uint16_t addr );

    void     mapPage( uint16_t addr, uint16_t mask, uint8_t rwx, void *page );
    void     mapRange( uint16_t addr, uint16_t end, uint16_t mask, uint8_t rwx, void *pages );

    uint8_t  read( uint16_t addr );
    void     write( uint16_t addr, uint8_t byte );
};



