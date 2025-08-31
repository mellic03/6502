#pragma once

#include <cstddef>
#include <cstdint>



// https://www.nesdev.org/wiki/INES

struct INESHeader
{
    char    signature[4];
    uint8_t prgRomSize;
    uint8_t chrRomSize;
    uint8_t flags6;
    uint8_t flags7;
    uint8_t flags8;
    uint8_t flags9;
    uint8_t flags10;
    uint8_t unused[5];
};


// void sdfdsf()
// {
//     sizeof(INESHeader);
// }
