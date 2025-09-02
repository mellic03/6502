#include "bus.hpp"
#include <stdio.h>
#include <cassert>


DataBus::DataBus()
{
    mDevices.push_back({nullptr, 0, nullptr, nullptr});
}

void DataBus::attach( iBusDevice *dev, uint16_t baseaddr, uint16_t maxaddr,
                      ReadFn rd, WriteFn wt )
{
    uint16_t idx = mDevices.size();
    for (uint32_t i=baseaddr; i<=maxaddr; i++)
        mDeviceMap[(uint16_t)i] = idx;
    mDevices.push_back({dev, baseaddr, rd, wt});
}

uint8_t *DataBus::_read( uint16_t addr )
{
    static uint8_t dummy = 0;
    auto &[dev, base, rdfn, wtfn] = mDevices[mDeviceMap[addr]];
    if (dev == nullptr)
        printf("[DataBus::_read] Bad read [0x%04X]\n", addr);
    assert(dev != nullptr);
    return rdfn(dev, base, addr);
}

uint8_t DataBus::read( uint16_t addr )
{
    return *_read(addr);
}

uint16_t DataBus::read16( uint16_t addr )
{
    return (read(addr+1) << 8) | read(addr+0);
}

void DataBus::write( uint16_t addr, uint8_t byte )
{
    auto &[dev, base, rdfn, wtfn] = mDevices[mDeviceMap[addr]];
    if (dev == nullptr) return;
    wtfn(dev, base, addr, byte);
}

void DataBus::write16( uint16_t addr, uint16_t word )
{
    write(addr, uint8_t(word & 0x00FF));
    write(addr, uint8_t(word >> 8));
}

// uint8_t BusInterface::rdbus( uint16_t addr )
// {
//     return mBus->mMem[deplex(addr)];
// }

// uint16_t BusInterface::rdbus16( uint16_t addr )
// {
//     return *(uint16_t*)(mBus->mMem + deplex(addr));
// }

// void BusInterface::wtbus( uint16_t addr, uint8_t byte )
// {
//     mBus->mMem[deplex(addr)] = byte;
// }

// void BusInterface::wtbus16( uint16_t addr, uint16_t word )
// {
//     *(uint16_t*)(mBus->mMem + deplex(addr)) = word;
// }

