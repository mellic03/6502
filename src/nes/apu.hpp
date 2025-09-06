#pragma once

#include "../hw/bus.hpp"
#include "../hw/tmemory.hpp"



class NesAPU: public HwDevice
{
private:
    uint8_t mPage[0x0100];

public:
    virtual void tick( uint64_t dt ) final {  };

};


