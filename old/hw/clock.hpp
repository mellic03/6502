#pragma once
#include "hw.hpp"


namespace emu
{
    struct clock_device;
}


struct emu::clock_device: public emu::hw_base
{
private:
    void (*_Tick)(void*);

public:
    size_t mCycleCount;

    clock_device( void (*TickFn)(void *arg) )
    :   _Tick(TickFn), mCycleCount(0) {  }

    void Tick( void *arg )
    {
        if (_Tick != nullptr)
        {
            _Tick(arg);
            mCycleCount += 1;
        }
    }
};

