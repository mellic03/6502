#include "comm.hpp"
#include "bus.hpp"


void emu::comm_device::OnMessage( uint8_t byte )
{
    if (_OnMessage)
    {
        _OnMessage(this, byte);
    }
}


emu::mmio_device::mmio_device( size_t mmap_size, emu::addr_device &bus )
:   mAddrBegin(bus.resv_range(mmap_size)),
    mAddrEnd(mAddrBegin + mmap_size)
{
    
}
