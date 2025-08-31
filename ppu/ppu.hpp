#pragma once

#include "../system/bus.hpp"
#include "../system/clock.hpp"



class DataBusPPU: public iDataBus
{
private:
public:
    struct MMapPPU
    {
        uint8_t vrom[0x2000]; // 0x0000 -> 0x2000
        uint8_t vram[0x1F00]; // 0x2000 -> 0x3F00
        uint8_t data[0x0100]; // 0x3F00 -> 0x4000
    } *mMap;

    DataBusPPU()
    :   iDataBus(new MMapPPU())
    {
        mMap = (MMapPPU*)mMem;
    }
};

struct PPURegisters
{

};

class BusInterfacePPU: public BusInterface
{
private:
    uint16_t deplex( uint16_t addr )
    {
        return addr % 0x4000;
    }

    uint8_t rdpalette( uint16_t addr )
    {

    }
};



class BusAttachmentPPU: public BusAttachment<BusInterfacePPU>, public SignalListener
{
private:
    struct MMapPPU {
        uint8_t PtrnTab[0x2000]; // 0x0000 -> 0x2000
        uint8_t NameTab[0x1F00]; // 0x2000 -> 0x3F00
        uint8_t Colours[0x0100]; // 0x3F00 -> 0x4000
    } *mMap;

public:
    BusAttachmentPPU( iDataBus* );
    virtual void Tick() final;

    // void ersser()
    // {
    //     auto *bus = (DataBusPPU*)(mBus.mBus);
    //     bus->mMap.
    // }
};


