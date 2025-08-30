#include "bus.hpp"
#include "comm.hpp"


size_t
emu::addr_device::resv_range( size_t size )
{
    static size_t tail = 0x00;
    size_t head = tail;
    tail += size;
    return head;
}


void
emu::addr_device::send( uint16_t port, uint8_t byte )
{
    for (comm_device *dev: mDevices[port])
    {
        if (port == dev->mListenPort)
        {
            dev->OnMessage(byte);
        }
    }
}


void
emu::addr_device::broadcast( uint8_t byte )
{
    for (auto &[key, devices]: mDevices)
    {
        for (comm_device *dev: devices)
        {
            dev->OnMessage(byte);
        }
    }
}

