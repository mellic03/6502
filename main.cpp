#include <iostream>
#include <fstream>

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "6502.hpp"


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        printf("Usage: assembler [filepath]\n");
        return 1;
    }

    std::ifstream stream(argv[1], std::ifstream::binary);
    if (!stream.is_open())
    {
        printf("Could not open file \"%s\"\n", argv[1]);
        return 1;
    }


    stream.seekg(0, std::ifstream::end);
    size_t size = stream.tellg();
    stream.seekg(0, std::ifstream::beg);
    
    cpu6502 cpu;
    stream.read((char*)(cpu.mMem), size);
    cpu.Run();

    return 0;
}

