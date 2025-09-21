#pragma once

#include <memu/types.hpp>
#include <vector>
#include <memory>

namespace NesEmu
{
    class GamePak;
}


class NesEmu::GamePak
{
private:
    std::vector<ubyte> mBuf;

public:
    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };
    GamePak( const std::string &path );

    uint8_t *data() { return mBuf.data(); }
    size_t   size() { return mBuf.size(); }
};

