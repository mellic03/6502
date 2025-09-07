#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "../hw/memory.hpp"
#include "file/ines.hpp"
#include "file/nes20.hpp"


namespace NesEmu
{
    class GamePak;
}


class NesEmu::GamePak
{
public:
    std::vector<ubyte> mRawFile;
    uint32_t mMapperNo;

    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };
    // MemoryRO mPrgROM;
    // MemoryRW mPrgRAM;
    // MemoryRO mChrROM;
    // iNES_File *mFile;

    GamePak( const std::string &path );
};

