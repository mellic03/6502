#pragma once

namespace NesEmu
{
    class System;
    class Cartridge;

    namespace NesMem
    {
        using MapFunc = void(*)(System&, Cartridge*);
        extern MapFunc *MapperFuncs; 

        void Mapper00( System&, Cartridge* );
        void Mapper01( System&, Cartridge* );
        void Mapper02( System&, Cartridge* );
        void Mapper03( System&, Cartridge* );
    }
}

