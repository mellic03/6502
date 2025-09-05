#pragma once

#include <stddef.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <functional>


class DataBus;
class iBusDevice;
class iBusAddrMap;


class BusDevice
{
public:
    uint8_t (*read)(uint16_t);
    void    (*write)(uint16_t, uint8_t);
};


class DataBus
{
public:
    using ReadFn  = uint8_t* (*)(iBusDevice*, uint16_t base, uint16_t addr);
    using WriteFn = void (*)(iBusDevice*, uint16_t base, uint16_t, uint8_t);

private:
    using ReadFunc  = std::function<uint8_t(uint16_t addr)>;
    using WriteFunc = std::function<void(uint16_t addr, uint8_t byte)>;
    struct FPair { ReadFunc read; WriteFunc write; };
    std::vector<FPair> mPageFuncs;
    uint8_t mPageTable[0xFFFF + 1];
    // uint8_t mPageTable[(0xFFFF + 1) / 256];
    std::set<iBusDevice*> mDevices;

public:
    DataBus();
    void map( uint16_t min, uint16_t max, ReadFunc, WriteFunc );
    void attach( iBusDevice* );
    void tick();

    uint8_t read( uint16_t addr );
    void    write( uint16_t addr, uint8_t byte );
};


class iBusDevice
{
public:
    virtual void Tick() = 0;
};

