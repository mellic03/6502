#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "../hw/memory.hpp"
#include "file/ines.hpp"
#include "file/nes20.hpp"
#include "mapper.hpp"


namespace NesEmu
{
    class GamePak;
}


class NesEmu::GamePak
{
public:
    std::vector<ubyte> mFile;
    std::vector<ubyte> mPrgROM;
    std::vector<ubyte> mChrROM;
    iNES_Header       *mHeader;
    ubyte             *mData;

    uint32_t           mMapNo;
    NesEmu::Mapper    *mMapper;

    bool is_bad() { return mFile.empty(); }

    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };
    // MemoryRO mPrgROM;
    // MemoryRW mPrgRAM;
    // MemoryRO mChrROM;
    // iNES_File *mFile;

    GamePak( const std::string &path );
};

