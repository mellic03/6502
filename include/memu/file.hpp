#pragma once

#include <string>
#include <vector>
#include "types.hpp"

namespace memu
{
    size_t loadFileRaw( const std::string &path, void *buf, size_t bufsz );
    std::vector<uint8_t> loadFileRaw( const std::string &path );
}

