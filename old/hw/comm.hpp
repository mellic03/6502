#pragma once
#include "hw.hpp"

namespace emu
{
    struct addr_device;
}

namespace emu
{
    struct comm_device;
    struct comm_listener;
    struct mmio_device;
}


struct emu::comm_device: public emu::hw_base
{
private:
    using onmsg_t = void (*)(void*, uint8_t);
    const onmsg_t _OnMessage;
    
public:
    uint16_t mListenPort;

    comm_device( uint16_t ListenPort, onmsg_t OnMsgFptr )
    :   mListenPort(ListenPort), _OnMessage(OnMsgFptr) {  }

    void OnMessage( uint8_t byte );
};


struct emu::mmio_device
{
    const size_t mAddrBegin;
    const size_t mAddrEnd;

    mmio_device( size_t mmap_size, emu::addr_device &bus );
};




struct emu::comm_listener
{
private:
    using onmsg_t = void (*)(void*, uint8_t);
    const onmsg_t _OnMessage;
    
public:
    uint16_t mListenPort;

    comm_listener( uint16_t ListenPort, onmsg_t OnMsgFptr )
    :   mListenPort(ListenPort), _OnMessage(OnMsgFptr) {  }

    void OnMessage( uint8_t byte );
};




struct comm_device
{
    std::vector<emu::comm_listener*> mListeners;

    void attach( emu::comm_listener *listener )
    {
        mListeners.push_back(listener);
    }

    void send( uint16_t port, uint8_t byte )
    {
        for (auto *listener: mListeners)
        {
            if (port == listener->mListenPort)
            {
                listener->OnMessage(byte);
            }
        }
    }

    void broadcast( uint8_t byte )
    {
        for (auto *listener: mListeners)
        {
            listener->OnMessage(byte);
        }
    }

};
