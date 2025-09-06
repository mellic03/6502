#pragma once

#include <cstddef>
#include <cstdint>


namespace NesFile
{
    struct Base;
}


struct NesFile::Base
{
public:
    uint8_t *mData;
    size_t   mSize;

    Base( uint8_t *raw = nullptr, size_t size = 0)
    :   mData(raw), mSize(size) {  }
};
