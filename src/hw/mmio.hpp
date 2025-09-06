#pragma once

#include "bus.hpp"
#include "memory.hpp"
#include "tmemory.hpp"
#include "../rwx.hpp"


template <typename EnumType>
struct mmioDevice: public ioDevice
{
private:
    // struct mmio_entry_t { uint8_t val, rwx; };

public:

    virtual void tick( uint64_t dt ) override
    {

    };

    uint8_t mmio_read( EnumType tag )
    {
        uint8_t idx = (uint8_t)tag;
        if (idx & RWX::R)
            return iobuf()[idx];
        return 0;
    }

    void mmio_write( EnumType tag, uint8_t value )
    {
        uint8_t idx = (uint8_t)tag;
        if (idx & RWX::W)
            iobuf()[idx] = value;
        return;
    }
};



// template <typename Tag, typename Rwx>
// struct NesEmu::RegisterInplaceMMIO
// {
// private:
//     uint8_t *mRegs;

// public:
//     RegisterInplaceMMIO( uint8_t *base )
//     :   mRegs(base)
//     {
//         static_assert( Tag::NumValues == Rwx::NumValues );
//     }

//     uint8_t rd( Tag tag )
//     {
//         uint8_t idx = (uint8_t)tag;
//         if ((Rwx)idx & RWX::R)
//             return mRegs[idx];
//         return 0;
//     }

//     void wt( Tag tag, uint8_t value )
//     {
//         uint8_t idx = (uint8_t)tag;
//         if ((Rwx)idx & RWX::W)
//             mRegs[idx] = value;
//         return;
//     }
// };

