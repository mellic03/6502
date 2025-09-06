#pragma once

#include "bus.hpp"
class DataBus;


class HwDevice
{
public:
    DataBus *mBus = nullptr;

    uint8_t rdbus( uint16_t );
    void    wtbus( uint16_t, uint8_t );
    virtual void tick( uint64_t dt ) = 0;
};



class ioDevice: public HwDevice
{
private:
    uint8_t mPage[0x0100];

public:
    uint8_t  io_read(uint8_t i) { return mPage[i]; }
    void     io_write(uint8_t i, uint8_t x) { mPage[i] = x; }
    uint8_t *io_base() { return &mPage[0]; }

    virtual void tick( uint64_t dt ) = 0;
    virtual uint8_t rd( uint16_t a ) { return mPage[a]; };
    virtual void wt( uint16_t a, uint8_t x ) { mPage[a] = x; };

};