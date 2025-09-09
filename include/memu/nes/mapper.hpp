#pragma once
#include "file.hpp"
#include "gamepak.hpp"

namespace NesEmu
{
    class System;
    class Mapper;
}


class NesEmu::Mapper
{
public:
    static Mapper *MapGamePak(NesEmu::System&, GamePak*);
};

