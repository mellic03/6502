#pragma once

#include <stddef.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <functional>
#include "../rwx.hpp"



class HwDevice;
// class DataBus;


class DataBus
{
private:
    // using DcFun = uint16_t (*)(uint16_t);
    // using RdFun  = uint8_t (*)(HwDevice*, uint16_t);
    // using WtFun = void (*)(HwDevice*, uint16_t, uint8_t);

    // struct FuncQuad
    // {
    //     HwDevice *dev;
    //     DcFun dc;
    //     RdFun rd;
    //     WtFun wt;
    // };

    // std::vector<FuncQuad> mPageFuncs;
    // uint8_t mPageTable[0xFFFF + 1];

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
    void tick();
    void attach( HwDevice* );

    const uint8_t *getReadPtr( uint16_t addr );
          uint8_t *getWritePtr( uint16_t addr );

    void     mapPage( uint16_t addr, uint16_t mask, uint8_t rwx, uint8_t *page );
    void     mapRange( uint16_t addr, uint16_t end, uint16_t mask, uint8_t rwx, uint8_t *pages );
    uint8_t  read( uint16_t addr );
    void     write( uint16_t addr, uint8_t byte );
};






class HwDevice
{
public:
    DataBus *mBus = nullptr;

    uint8_t rdbus( uint16_t );
    void    wtbus( uint16_t, uint8_t );
    virtual void Tick() = 0;
};

