#include <memu/nes/gamepak.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>


NesEmu::GamePak::GamePak( const std::string &path )
{
    mBuf = memu::loadFileRaw(path);
    memu::log::Info("mBuf.size() == %lu\n", mBuf.size());
}
