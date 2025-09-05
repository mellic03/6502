#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>


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

    std::unique_ptr<NesFile::File> mFile;
    uint8_t mMapNo = 0;

    Cartridge( const std::string &path );
    bool is_bad() { return (mFile.get() == nullptr); }

};

    size_t headerSz = 16;
    size_t trainerSz;
    size_t prgRomSz;
    size_t chrRomSz;
    size_t pcInstRomSz;
    size_t pcPRomSz;
    size_t titleSz;