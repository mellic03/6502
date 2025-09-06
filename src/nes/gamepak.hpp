#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>

#include "../hw/memory.hpp"
#include "file/ines.hpp"
#include "file/nes20.hpp"


namespace NesEmu
{
    class GamePak;
    class System;
    struct FileU;
}


class NesEmu::GamePak: public ioDevice
{
private:
    // std::unique_ptr<uint8_t[]> mData;

public:
    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };

    uint8_t         mFmt;
    NesFile::iNES  *miNES;
    NesFile::NES20 *mNES20;

    GamePak( const std::string &path );
    bool is_bad() { return (mData == nullptr); }
};


// struct NesEmu::FileU
// {
//     uint8_t fmt;

//     union {
//         NesFile::Base  as_Base;
//         NesFile::iNES  as_iNES;
//         NesFile::NES20 as_NES20;
//     };

//     FileU( uint8_t *raw, size_t size );
// };