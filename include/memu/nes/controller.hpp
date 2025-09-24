#pragma once

#include <memu/types.hpp>


struct NesController
{
    ubyte a     :1;
    ubyte b     :1;
    ubyte sel   :1;
    ubyte start :1;
    ubyte up    :1;
    ubyte down  :1;
    ubyte left  :1;
    ubyte right :1;
};

