#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"
#include "../hw/tmemory.hpp"
#include "../rwx.hpp"

namespace NesEmu
{
    template <typename TagEnum, typename AccessEnum>
    struct RegisterMMIO;

    // template <typename TagEnum, typename AccessEnum>
    // struct RegisterInplaceMMIO;
}


template <typename Tag, typename Rwx>
struct NesEmu::RegisterMMIO: public HwDevice
{
private:
    // struct mmio_entry_t { uint8_t val, rwx; };
    uint8_t *mPage;

public:

    RegisterMMIO()
    :   mPage(new uint8_t[256])
    {
        static_assert((int)Tag::NumValues == (int)Rwx::NumValues);
    }

    virtual void tick() override
    {

    };

    uint8_t *data()
    {
        return mPage;
    }

    uint8_t rd( Tag tag )
    {
        uint8_t idx = (uint8_t)tag;
        if ((Rwx)idx & RWX::R)
            return mPage[idx];
        return 0;
    }

    void wt( Tag tag, uint8_t value )
    {
        uint8_t idx = (uint8_t)tag;
        if ((Rwx)idx & RWX::W)
            mPage[idx] = value;
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

