#pragma once
#include <memu/hw/mostech/6502.hpp>


class Ricoh2A03: public m6502
{
public:
    using m6502::m6502;

    struct StdCtl
    {
    private:
        uint8_t sigprev = 0;
        uint8_t sigcurr = 0;
        uint8_t bitidx  = 0;

        union {
            uint8_t buf[8];
            struct {
                uint8_t a  ; // :1;
                uint8_t b  ; // :1;
                uint8_t sel; // :1;
                uint8_t str; // :1;
                uint8_t up ; // :1;
                uint8_t dwn; // :1;
                uint8_t l  ; // :1;
                uint8_t r  ; // :1;
            };
        };

    public:
        void lo()
        {
            sigprev = sigcurr;
            sigcurr = 0;
            if (sigprev == 1)
                bitidx = 0;
        }

        void hi()
        {
            sigprev = sigcurr;
            sigcurr = 1;
        }

        uint8_t read()
        {
            uint8_t bit = (buf[bitidx]) ? 1 : 0;
            bitidx = (bitidx+1) & 0x07;
            return bit;
        }

    } mStdCtl0, mStdCtl1;

};

using NesCPU = Ricoh2A03;

