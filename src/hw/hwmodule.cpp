#include "hwmodule.hpp"
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <cstdlib>



Emu::HwModule::HwModule( Emu::AddrSpace &bus )
:   mBus(bus)
{

}


// void Emu::HwModule::tick( uint64_t dt )
// {
//     for (HwDevice *dev: mDevices)
//     {
//         dev->tick(dt);
//     }
// }


// void Emu::HwModule::attach( HwDevice *dev )
// {
//     dev->mBus = this;
//     mDevices.insert(dev);
// }
