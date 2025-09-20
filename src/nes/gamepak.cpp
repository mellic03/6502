#include <memu/nes/gamepak.hpp>
#include <memu/log.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>

static uint8_t rdFmt( uint8_t *rom )
{
    using Fmt = NesEmu::GamePak::Fmt;
    uint8_t fmt = Fmt::OTHER;

    if (strncmp("NES\x1A", (char*)rom, 4) == 0)
        fmt = Fmt::iNES;

    if (fmt==Fmt::iNES && (rom[7] & 0x0C)==0x08)
        fmt = Fmt::NES20;

    return fmt;
}


NesEmu::GamePak::GamePak( const std::string &path )
{
    std::ifstream stream(path, std::ifstream::binary);

    if (!stream.is_open())
    {
        printf("Could not open file \"%s\"\n", path.c_str());
        exit(1);
    }

    stream.seekg(0, std::ifstream::end);
    m_buf.resize(stream.tellg());

    stream.seekg(0, std::ifstream::beg);
    stream.read((char*)(m_buf.data()), m_buf.size());

    logasrt(m_buf.size() > 0, "Gpak::size() == %lu", this->size());
}
