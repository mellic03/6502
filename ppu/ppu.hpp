#pragma once

#include "../system/bus.hpp"
#include "../system/clock.hpp"


class DataBusPPU: public iDataBus
{
private:
    struct MMapPPU {
        uint8_t PtrnTab[0x2000]; // 0x0000 -> 0x2000
        uint8_t NameTab[0x1F00]; // 0x2000 -> 0x3F00
        uint8_t Colours[0x0100]; // 0x3F00 -> 0x4000
    } *mMap;

public:
    DataBusPPU()
    :   iDataBus(new MMapPPU())
    {
        mMap = (MMapPPU*)mMem;
    }
};




class BusInterfacePPU: public BusInterface
{
private:
    uint16_t deplex( uint16_t addr )
    {
        return addr % 0x4000;
    }
};




class BusAttachmentPPU: public BusAttachment<BusInterfacePPU>, public SignalListener
{
private:

public:
    BusAttachmentPPU( iDataBus* );
    virtual void Tick() final;
};


