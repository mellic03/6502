#pragma once
#include <memu/types.hpp>

namespace m6502_detail
{
    class BaseHw;
};


class m6502_detail::BaseHw
{
public:
    BaseHw()
    : AC(0), XR(0), YR(0), SP(0xFD), PC{0xFFFC}, SSR{0b00100100} {  };

// protected:
    ubyte AC, XR, YR, SP;

    union {
        uword PC;
        struct {
            ubyte PC_lo;
            ubyte PC_hi;
        } __attribute__((packed));
    };

    union {
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
    } SSR;

};

// enum class CpuPin: uint8_t
// {
//     IRQ, RES, NMI, WAI,
//     NumValues
// };


// class m6502_detail::BaseHwSig: public memu::SigPins<CpuPin>
// {
// public:
//     BaseHwSig(): m_sigcurr{0b0011}, m_sigprev{0b0011} {  };

// // protected:
//     // union {
//     //     ubyte as_byte;
//     //     struct {
//     //         uint8_t irq :1;
//     //         uint8_t res :1;
//     //         uint8_t nmi :1;
//     //         uint8_t wai :1;
//     //         uint8_t uuu :4;
//     //     }  __attribute__((packed));
//     // } m_sigprev, m_sigcurr;

//     //                  00000001,  00000010,  00000100,  00001000
//     //                  ____W___,  _____N__,  ______R_,  _______I
//     //                  00000000,  ________,  ____WNRI,  ____W___
//     //

//     // void sigLow (PIN_ p) { m_sigcurr.as_byte &= ~(1 << (uint8_t)p); }
//     // void sigHigh(PIN_ p) { m_sigcurr.as_byte |=  (1 << (uint8_t)p); }
//     // void sigFlip(PIN_ p) { m_sigcurr.as_byte ^=  (1 << (uint8_t)p); }

// };

