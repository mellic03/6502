#include "6502.hpp"


void cpu6502::InstrUnimp( uint8_t* )
{
    mInvalidOp = 1;
}


/*
    Carry Flag:
        - When addition result is 0 to 255, C=0.
        - When addition result is greater than 255, C=1.
        - When subtraction result is 0 to 255, C=1.
        - When subtraction result is less than 0, C=0.
*/
// #include <algorithm>
// template<size_t N>
// struct StringLiteral {
//     constexpr StringLiteral(const char (&str)[N]) {
//         std::copy_n(str, N, value);
//     }
    
//     char value[N];
// };

// template <StringLiteral mask> // rooo8<"N-------">(AC);
// void rooo8( uint16_t word )
// {
//     if constexpr (mask.value[0] == 'N')
//     {

//     }
// }

template <uint8_t mask>
void cpu6502::_setssr( uint16_t word )
{
    static constexpr uint8_t BIT_C = 1<<0;
    static constexpr uint8_t BIT_Z = 1<<1;
    static constexpr uint8_t BIT_V = 1<<6;
    static constexpr uint8_t BIT_N = 1<<7;

    auto byte = mask;
    auto ssr  = *(cpu6502RegisterSSR*)(&byte);

    uint8_t lo  = (uint8_t)(word & 0x00FF);
    uint8_t hi  = (uint8_t)(word >> 8);

    if constexpr (mask & BIT_C) // C
    {
        SSR.C = (word & 1) ? 1 : 0;
    }

    if constexpr (mask & BIT_Z) // Z
    {
        SSR.Z = (lo == 0) ? 1 : 0;
    }

    // if constexpr (mask & BIT_V) // V
    // {
    //     SSR.V = (hi) ;
    // }

    if constexpr (mask & BIT_N) // N
    {
        SSR.N = (lo & 0b1000'0000) ? 1 : 0;
    }
}

// void cpu6502::InstrADC( uint8_t *src )
// {
//     uint16_t word = (uint16_t)AC + src;
//     _setssr<0b11000011>(word);
//     AC = (uint8_t)word;
// }

/*
    N (Negative): Mirrors the top bit of the result after an arithmetic operation.
    V (Overflow): Set during ADC/SBC if overflow occurs.
    1: Reserved and typically set to 1.
    B (Break): Differentiates hardware (IRQ/NMI) from software (BRK) interrupts.
    D (Decimal): Enables BCD mode for arithmetic operations.
    I (Interrupt): Disables interrupts (IRQ).
    Z (Zero): Indicates if the result is zero.
    C (Carry): Acts as a 9th bit for addition/subtraction and bit shifts.
*/

void cpu6502::InstrADC( uint8_t *src )
{
    static constexpr uint8_t BIT7 = (1<<7);

	// uint16_t res = ((uint16_t)AC + x) + SSR.C;

    // if (SSR.D)
    // {
    //     static constexpr uint8_t AAA0 = 9;
    //     static constexpr uint8_t AAA1 = 0x09;
    //     static constexpr uint8_t AAA2 = 0b0000'1001;
    //     // result
    //     _setssr<0b10000011>(x);
    // }

    // else
    // {
    //     _setssr<0b10000011>(x);
    //     // SSR.V = (!(AC & BIT7) && (res & BIT7)) ? 1 : 0;
    // }

    uint8_t x = *src;
    // SSR.V = not(  ((A7 NOR B7) and C6)  NOR  ((A7 NAND B7) NOR C6)  )
    bool C6 = bool(SSR.C);
    bool A7 = bool(AC & BIT7);
    bool B7 = bool(x  & BIT7);
    SSR.V = (!(A7||B7) && C6) || !(!(A7 && B7) || C6);

    _setssr<0b10000011>((uint16_t)AC + x);
    AC = uint8_t(uint16_t(AC) + x);
}


void cpu6502::InstrAND( uint8_t *src )
{
    AC &= *src;
    SSR.N = (AC) ? 1 : 0;
    SSR.Z = (AC) ? 0 : 1;
}



void cpu6502::InstrBCC( uint8_t *src )
{
    if (SSR.C == 0) PC = *(uint16_t*)src;
}

void cpu6502::InstrBCS( uint8_t *src )
{
    if (SSR.C == 1) PC = *(uint16_t*)src;
}

void cpu6502::InstrBEQ( uint8_t *src )
{
    if (SSR.Z == 1) PC = *(uint16_t*)src;
}

void cpu6502::InstrBIT( uint8_t *src )
{
    union {
        uint8_t byte;
        cpu6502RegisterSSR ssr;
    } U = { *src };

    SSR.N = U.ssr.N;
    SSR.V = U.ssr.V;
}

void cpu6502::InstrBMI( uint8_t *src )
{
    if (SSR.N == 1) PC = *(uint16_t*)src;
}

void cpu6502::InstrBNE( uint8_t *src )
{
    if (SSR.Z == 0) PC = *(uint16_t*)src;
}

void cpu6502::InstrBPL( uint8_t *src )
{
    if (SSR.N == 0) PC = *(uint16_t*)src;
}

void cpu6502::InstrBRK( uint8_t *src )
{
    push16(PC+2);
    SSR.B = 1;
    push08(SSR_byte);
}

void cpu6502::InstrBVC( uint8_t *src )
{
    if (SSR.V == 0) PC = *(uint16_t*)src;
}

void cpu6502::InstrBVS( uint8_t *src )
{
    if (SSR.V == 1) PC = *(uint16_t*)src;
}

void cpu6502::InstrCLC( uint8_t *src )
{
    SSR.C = 0;
}

void cpu6502::InstrCLD( uint8_t *src )
{
    SSR.D = 0;
}

void cpu6502::InstrCLI( uint8_t *src )
{
    SSR.I = 0;
}

void cpu6502::InstrCLV( uint8_t *src )
{
    SSR.V = 0;
}

void cpu6502::InstrCMP( uint8_t *src )
{
    uint16_t res = (uint16_t)AC - *src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrCPX( uint8_t *src )
{
    uint16_t res = (uint16_t)XR - *src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrCPY( uint8_t *src )
{
    uint16_t res = (uint16_t)YR - *src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrEOR( uint8_t *src )
{
    AC ^= *src;
    _setssr<0b10000010>(AC);
}

void cpu6502::InstrJMP( uint8_t *src )
{
    PC = *(uint16_t*)src;
}

void cpu6502::InstrJSR( uint8_t *src )
{
    push16(PC+2);
    PC = *(uint16_t*)src;
}

void cpu6502::InstrLDA( uint8_t *src )
{
    AC = *src;
    _setssr<0b10000010>(AC);
}

void cpu6502::InstrLDX( uint8_t *src )
{
    XR = *src;
}

void cpu6502::InstrLDY( uint8_t *src )
{
    YR = *src;
}

void cpu6502::InstrNOP( uint8_t* )
{

}

void cpu6502::InstrORA( uint8_t *src )
{
    AC |= *src;
    _setssr<0b10000010>(AC);
    // SSR.N = ( AC) ? 1 : 0;
    // SSR.Z = (!AC) ? 1 : 0;
}

void cpu6502::InstrPHA( uint8_t* )
{
    // mBus[SP++] = AC;
    push08(AC);
}

void cpu6502::InstrPHP( uint8_t* )
{
    union {
        cpu6502RegisterSSR ssr;
        uint8_t byte;
    } U = { SSR };

    U.ssr.B = 1;
    U.ssr._ = 1;
    push08(U.byte);
}

void cpu6502::InstrPLA( uint8_t* )
{
    // AC = mBus[--SP];
    AC = pop08();
}

void cpu6502::InstrPLP( uint8_t* )
{
    union {
        uint8_t byte;
        cpu6502RegisterSSR ssr;
    } U = { pop08() };

    U.ssr.B = SSR.B;
    U.ssr._ = SSR._;
    SSR = U.ssr;
}

void cpu6502::InstrRTI( uint8_t* )
{
    auto tmp = SSR;
    InstrPLP(nullptr);
    SSR.B = tmp.B;
    SSR._ = tmp._;

    PC = pop16();
}

void cpu6502::InstrRTS( uint8_t* )
{
    PC = pop16() + 1;
}

void cpu6502::InstrSBC( uint8_t *x )
{
    uint8_t onecomp = ~uint8_t(*x);
    uint8_t twocomp = ~uint8_t(*x) + 1;
    InstrADC(&twocomp);
}

void cpu6502::InstrSEC( uint8_t* )
{
    SSR.C = 1;
}

void cpu6502::InstrSTA( uint8_t *x )
{
    *x = AC;
}

void cpu6502::InstrSTX( uint8_t *x )
{
    *x = XR;
}

void cpu6502::InstrSTY( uint8_t *x )
{
    *x = YR;
}


