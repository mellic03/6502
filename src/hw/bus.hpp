#pragma once

#include <stddef.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <functional>


class DataBus;
class iBusDevice;
class iBusAddrMap;


class iBusDevice
{
public:
    DataBus *mBus = nullptr;
    virtual void Tick() = 0;
};



class DataBus
{
private:
    using DcFun = uint16_t (*)(uint16_t);
    using RdFun  = uint8_t (*)(iBusDevice*, uint16_t);
    using WtFun = void (*)(iBusDevice*, uint16_t, uint8_t);

    struct FuncQuad
    {
        iBusDevice *dev;
        DcFun dc;
        RdFun rd;
        WtFun wt;
    };

    std::set<iBusDevice*> mDevices;
    std::vector<FuncQuad> mPageFuncs;
    uint8_t mPageTable[0xFFFF + 1];
    // uint8_t mPageTable[(0xFFFF + 1) / 256];

public:
    DataBus();
    
    void attach( iBusDevice* );
    void map( iBusDevice *dev, uint16_t min, uint16_t max, DcFun, RdFun, WtFun );
    void map( iBusDevice *dev, uint16_t min, uint16_t max, RdFun, WtFun );
    // void map( uint16_t min, uint16_t max, ReadFunc, WriteFunc );
    void tick();

    uint8_t read( uint16_t addr );
    void    write( uint16_t addr, uint8_t byte );
};

