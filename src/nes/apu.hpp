#pragma once

#include "../hw/bus.hpp"
#include "../hw/tmemory.hpp"


class NesAPU: public iBusDevice
{
public:
    uint8_t mTestMem[36];
    NesAPU() {  };
    virtual void Tick() final {  };
};




