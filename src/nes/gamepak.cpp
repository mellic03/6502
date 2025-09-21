#include <memu/nes/gamepak.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>


NesEmu::GamePak::GamePak( const std::string &path )
{
    std::ifstream stream(path, std::ifstream::binary);

    if (!stream.is_open())
    {
        printf("Could not open file \"%s\"\n", path.c_str());
        exit(1);
    }

    mBuf = memu::loadFileRaw(path);

    LogAsrt(mBuf.size() > 0, "Gpak::size() == %lu\n", mBuf.size());
}
