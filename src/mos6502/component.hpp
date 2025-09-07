#pragma once
#include "../types.hpp"


namespace MOS_6502
{
    union RegPC;
    union RegSSR;
    union SignalPins;
}


union MOS_6502::RegPC
{
    uword word;
    struct {
        uint8_t PC_lo;
        uint8_t PC_hi;
    } __attribute__((packed));
};


union MOS_6502::RegSSR
{
    ubyte byte;
    struct {
        ubyte C :1;
        ubyte Z :1;
        ubyte I :1;
        ubyte D :1;
        ubyte U :1;
        ubyte B :1;
        ubyte V :1;
        ubyte N :1;
    } __attribute__((packed));
}; // 0b00100100


union MOS_6502::SignalPins
{
    ubyte byte;
    struct {
        uint8_t irq :1;
        uint8_t res :1;
        uint8_t nmi :1;
        uint8_t wai :1;
    };
}; // 0b0011

