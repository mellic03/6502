#pragma once

#include "bus.hpp"
class DataBus;


class HwDevice
{
public:
    DataBus *mBus = nullptr;

    uint8_t rdbus( uint16_t );
    void    wtbus( uint16_t, uint8_t );
    virtual void tick() = 0;
};



class ioDevice: public HwDevice
{
private:
    uint8_t mPage[0x0100];

public:
    virtual void tick() = 0;
    virtual uint8_t rd( uint16_t a ) { return mPage[a]; };
    virtual void wt( uint16_t a, uint8_t x ) { mPage[a] = x; };

};