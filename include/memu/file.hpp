#pragma once
#include <string>

namespace memu
{
    size_t loadFileRaw( const std::string &path, void *buf, size_t bufsz );
}

