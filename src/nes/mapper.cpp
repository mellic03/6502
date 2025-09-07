#include "mapper.hpp"

// struct MapperNROM;  // 00
// struct MapperMMC1;  // 01
// struct MapperUxROM; // 02
// struct MapperCNROM; // 03
// struct MapperMMC3;  // 04

NesEmu::Mapper *NesEmu::CreateMapper( int mapno, uint8_t *fileptr )
{
    switch (mapno)
    {
        default: break;;
        case 0: return new MapperNROM(fileptr);
        case 1: return new MapperMMC1(fileptr);
        case 2: return new MapperUxROM(fileptr);
        case 3: return new MapperCNROM(fileptr);
        case 4: return new MapperMMC3(fileptr);
    }
    return nullptr;
} 


NesEmu::Mapper::Mapper( ubyte *ptr )
:   mFile((iNES_File*)ptr)
{

}

