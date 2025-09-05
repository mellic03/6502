#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>

#include "file/ines.hpp"
#include "file/nes20.hpp"


namespace NesEmu
{
    class Cartridge;
    class System;
}

namespace NesFile
{
    struct File;
}


class NesEmu::Cartridge
{
public:
    struct Fmt
    {
        enum { iNES, NES20, OTHER, NumFmt };
    };

    uint8_t mFormat;
    uint8_t _readfmt( uint8_t *rom );

    std::unique_ptr<uint8_t[]> mRaw;

    union FileU {
        NesFile::Base  as_File;
        NesFile::iNES  as_iNES;
        NesFile::NES20 as_NES20;
    }; FileU *mFile;

    uint8_t mMapNo = 0;

    Cartridge( const std::string &path );
    bool is_bad() { return (mFile == nullptr); }

};

