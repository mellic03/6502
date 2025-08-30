#pragma once

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include <vector>
#include <map>

#include "comm.hpp"


namespace emu
{
    struct addr_device;

    template <typename T>
    struct bus_device;
}


struct emu::addr_device: public emu::hw_base
{
    const emu::BUSW_ mBusWidth;
    std::map<uint16_t, std::vector<comm_device*>> mDevices;

    addr_device( emu::BUSW_ BusWidth )
    :   mBusWidth(BusWidth) {  }

    size_t resv_range( size_t size );
    void send( uint16_t port, uint8_t byte );
    void broadcast( uint8_t byte );

    template <typename T>
    inline bool attach( bus_device<T> *dev )
    {
        if (dev == nullptr)
        {  return false; }

        if (dev->mBus != nullptr)
        {  return false; }

        mDevices[dev->mListenPort].push_back(dev);
        dev->mBus = this;

        return true;
    }
};



template <typename T>
struct emu::bus_device: public comm_device
{
    addr_device *mBus;
    uint16_t mAddrBase;
    uint16_t mAddrEnd;

    bus_device( void (*OnMessagePtr)(void*, uint8_t) = nullptr )
    :   comm_device(3022, OnMessagePtr), mBus(nullptr)  {  }

    uint8_t read( uint16_t addr )
    {
        assert(mBus != nullptr);
        return static_cast<T*>(this)->read_impl(addr);
    }

    void write( uint16_t addr, uint8_t byte )
    {
        assert(mBus != nullptr);
        static_cast<T*>(this)->write_impl(addr, byte);
    }
};




template <size_t ByteCapacity>
struct HwBusROM: public emu::bus_device<HwBusROM<ByteCapacity>>
{
    uint8_t mmio[ByteCapacity];

    uint8_t read_impl( uint16_t addr )
    {
        return mmio[addr];
    }

    void write_impl( uint16_t addr, uint8_t byte )
    {
        // 
    }
};



#include <stdio.h>

struct HwTestDevice1: public emu::bus_device<HwTestDevice1>
{
private:
    static void on_message( void *devptr, uint8_t byte )
    {
        HwTestDevice1 *dev = static_cast<HwTestDevice1*>(devptr);
        printf("[HwTestDevice1] Received %u\n", byte);
    }

public:
    HwTestDevice1()
    :   bus_device(HwTestDevice1::on_message) {  }

};

struct HwTestDevice2: public emu::bus_device<HwTestDevice2>
{
private:
    static void on_message( void *devptr, uint8_t byte )
    {
        HwTestDevice2 *dev = static_cast<HwTestDevice2*>(devptr);
        printf("[HwTestDevice2] Received %u\n", byte);
    }

public:
    HwTestDevice2()
    :   bus_device(HwTestDevice2::on_message) {  }

};



// namespace emu
// {
//     template <typename T>
//     inline bool AttachBusDevice( addr_device *bus, bus_device<T> *dev )
//     {
//         if (bus == nullptr)
//         {  return false; }

//         if (dev == nullptr)
//         {  return false; }

//         if (dev->mBus != nullptr)
//         {  return false; }

//         // if (dev->mBusWidth != bus->mBusWidth)
//         // {  return false; }

//         bus->mDevices[dev->mListenPort].push_back(dev);
//         dev->mBus = bus;

//         return true;
//     }
// }
