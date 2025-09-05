#pragma once

#include "../hw/bus.hpp"
#include "../hw/memory.hpp"
#include "../hw/tmemory.hpp"
#include "rwx.hpp"

namespace NesEmu
{
    template <typename TagEnum, typename AccessEnum>
    struct RegisterMMIO;

    enum class ExampleTag: uint8_t
    {
        REG_0, REG_1, REG_2, REG_3,
        REG_4, REG_5, REG_6, REG_7,
        NumValues
    };

    enum class ExampleRWX: uint8_t
    {
        REG_0, REG_1, REG_2, REG_3,
        REG_4, REG_5, REG_6, REG_7,
        NumValues
    };
}


template <typename Tag, typename Access>
struct NesEmu::RegisterMMIO
{
    struct mmio_entry_t { uint8_t val, rwx; };
    mmio_entry_t mRegs[Tag::NumValues];

    RegisterMMIO()
    {
        static_assert( Tag::NumValues == Access::NumValues );
    }

    uint8_t rd( Tag tag )
    {
        uint8_t idx = (uint8_t)tag;
        if ((Access)idx & RWX::R)
            return mRegs[idx].val;
        return 0;
    }

    void wt( Tag tag, uint8_t val )
    {
        uint8_t idx = (uint8_t)tag;
        if ((Access)idx & RWX::W)
            mRegs[idx].val = val;
        return;
    }
};

