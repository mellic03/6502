#include <memu/nes/gamepak.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>


NesEmu::GamePak::GamePak( const std::string &path )
{
    // std::ifstream stream(path, std::ifstream::binary);
    // LogAsrt(stream.is_open(), "Failed to open file \"%s\"\n", path.c_str());

    mBuf = memu::loadFileRaw(path);
    memu::log::Info("mBuf.size() == %lu\n", mBuf.size());

    // LogAsrt(mBuf.size() > 0, "Gpak::size() == %lu\n", mBuf.size());
}
