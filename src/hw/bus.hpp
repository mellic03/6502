#pragma once

#include "../types.hpp"
#include <set>
#include <vector>
#include <functional>

#include "device.hpp"
#include "../rwx.hpp"
using MemoryPage = uint8_t[256];


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

        PageTableEntry( uint8_t bse, uint8_t msk, uint8_t *pge, uint8_t rxwx )
        :   page(pge), base(bse), mask(msk), rwx(rxwx), in_use(1) {  }
    };

    std::set<HwDevice*> mDevices;
    PageTableEntry      mPageTables[256];

public:
    DataBus();

    void tick( uint64_t dt );
    void attach( HwDevice* );

    const ubyte *getReadPtr( uword addr );
    ubyte *getWritePtr( uword addr );

    void mapPage( uword addr, uword mask, void *page, ubyte rwx=RWX::RW );
    void mapRange( uword addr, uword end, uword mask, void *pages, ubyte rwx=RWX::RW );

    ubyte read( uword );
    void  write( uword, ubyte );
    uword operator[](uint16_t i) { return (uword)read(i); };

};



