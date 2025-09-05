#pragma once
#include "file.hpp"

namespace NesFile
{
    struct NES20: public NesFile::Base
    {
        NES20( uint8_t *raw = nullptr, size_t size = 0 );
    };
}
