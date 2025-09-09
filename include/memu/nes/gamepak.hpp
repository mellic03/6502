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
    std::vector<ubyte> m_buf;
    // std::vector<ubyte> mPrgROM;
    // std::vector<ubyte> mChrROM;
    // ubyte             *mData;

public:
    struct Fmt { enum { iNES, NES20, OTHER, NumFmt }; };
    GamePak( const std::string &path );

    uint8_t *data() { return m_buf.data(); }
    size_t   size() { return m_buf.size(); }
};

