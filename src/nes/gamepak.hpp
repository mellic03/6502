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
public:
    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };

    MemoryRO mPrgROM;
    MemoryRW mPrgRAM;
    MemoryRO mChrROM;
    // MemoryRW mChrRAM;
    uint8_t  mMapperNo;
    iNES_File *mFile;


    GamePak( const std::string &path );
    bool is_bad() { return (mData == nullptr); }
};

