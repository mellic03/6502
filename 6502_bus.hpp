#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


struct cpu6502Bus
{
private:
public:
    uint8_t *mem;

    uint16_t _DeplexAddr( uint16_t addr )
    {
        if (0x2000<=addr && addr <=0x3FFF)
        {
            addr = 0x2000 + (addr % 8);
        }

        return addr;
    }

    cpu6502Bus()
    :   mem(new uint8_t[65535]) {  }

    uint8_t &operator[]( uint16_t addr )
    {
        return mem[_DeplexAddr(addr)];
    }
};

