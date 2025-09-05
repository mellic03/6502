#include "nes20.hpp"
#include <cstring>
using namespace NesFile;


NesFile::NES20::NES20( uint8_t *raw, size_t size )
:   NesFile::Base(raw, size)
{
    return;
}

