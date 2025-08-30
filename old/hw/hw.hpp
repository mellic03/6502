#pragma once

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <new>
#include <vector>
#include <map>

#include "./types.hpp"


namespace emu
{
    struct hw_base;

    template <typename T, typename... Args>
    T *HwNew( Args... args )
    {
        return new T(args...);
    }
}


struct emu::hw_base
{

};

