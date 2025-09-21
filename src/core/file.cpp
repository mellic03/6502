#include <memu/file.hpp>
#include <memu/log.hpp>
#include <fstream>


size_t memu::loadFileRaw( const std::string &path, void *buf, size_t bufsz )
{
    std::ifstream stream(path, std::ifstream::binary);
    LogAsrt(stream.is_open(), "Could not open file \"%s\"\n", path.c_str());

    stream.seekg(0, std::ifstream::end);
    bufsz = std::min(bufsz, (size_t)(stream.tellg()));

    stream.seekg(0, std::ifstream::beg);
    stream.read((char*)buf, bufsz);

    return bufsz;
}


std::vector<uint8_t> memu::loadFileRaw( const std::string &path )
{
    std::ifstream stream(path, std::ifstream::binary);
    LogAsrt(stream.is_open(), "Could not open file \"%s\"\n", path.c_str());

    stream.seekg(0, std::ifstream::end);
    std::vector<uint8_t> buf(stream.tellg());

    stream.seekg(0, std::ifstream::beg);
    stream.read((char*)(buf.data()), buf.size());

    return buf;
}
