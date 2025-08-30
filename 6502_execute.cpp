#include "6502.hpp"


void cpu6502::InstrUnimp( uint16_t )
{
    mInvalidOp = 1;
}


void cpu6502::InstrADC( uint16_t src )
{
    AC += src;
    
}

void cpu6502::InstrAND( uint16_t src )
{
    AC &= src;
    SSR.N = (AC) ? 1 : 0;
    SSR.Z = (!AC) ? 1 : 0;
}



void cpu6502::InstrBCC( uint16_t src )
{
    if (SSR.C == 0)
        PC = src;
}

void cpu6502::InstrBCS( uint16_t src )
{
    if (SSR.C == 1) PC = src;
}

void cpu6502::InstrBEQ( uint16_t src )
{
    if (SSR.Z == 1) PC = src;
}

void cpu6502::InstrBIT( uint16_t src )
{

}

void cpu6502::InstrBMI( uint16_t src )
{
    if (SSR.N == 1) PC = src;
}

void cpu6502::InstrBNE( uint16_t src )
{
    if (SSR.Z == 0) PC = src;
}

void cpu6502::InstrBPL( uint16_t src )
{
    if (SSR.N == 0) PC = src;
}

void cpu6502::InstrBRK( uint16_t src )
{
    push16(PC+2);
    SSR.B = 1;
    push08(SSR_byte);
}

void cpu6502::InstrBVC( uint16_t src )
{
    if (SSR.V == 0) PC = src;
}

void cpu6502::InstrBVS( uint16_t src )
{
    if (SSR.V == 1) PC = src;
}

void cpu6502::InstrCLC( uint16_t src )
{
    SSR.C = 0;
}

void cpu6502::InstrCLD( uint16_t src )
{
    SSR.D = 0;
}

void cpu6502::InstrCLI( uint16_t src )
{
    SSR.I = 0;
}

void cpu6502::InstrCLV( uint16_t src )
{
    SSR.V = 0;
}

void cpu6502::InstrCMP( uint16_t src )
{
    uint16_t res = (uint16_t)AC - src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrCPX( uint16_t src )
{
    uint16_t res = (uint16_t)XR - src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrCPY( uint16_t src )
{
    uint16_t res = (uint16_t)YR - src;
    uint8_t  lo  = (uint8_t)(res & 0x00FF);
    uint8_t  hi  = (uint8_t)(res >> 8);

    SSR.N = (lo) ? 1 : 0;
    SSR.Z = (lo) ? 0 : 1;
    SSR.C = (hi) ? 1 : 0;
}

void cpu6502::InstrEOR( uint16_t src )
{

}


void cpu6502::InstrJMP( uint16_t src )
{
    PC = src;
}

void cpu6502::InstrJSR( uint16_t src )
{
    push16(PC+2);
    PC = src;
}

void cpu6502::InstrLDA( uint16_t src )
{
    AC = (uint8_t)src;
}

void cpu6502::InstrLDX( uint16_t src )
{
    XR = (uint8_t)src;
}

void cpu6502::InstrLDY( uint16_t src )
{
    XR = (uint8_t)src;
}

void cpu6502::InstrORA( uint16_t src )
{
    AC |= src;
    SSR.N = (AC) ? 1 : 0;
    SSR.Z = (!AC) ? 1 : 0;
}

void cpu6502::InstrPHA( uint16_t src )
{
    push08(AC);
}

void cpu6502::InstrPHP( uint16_t src )
{
    union {
        cpu6502RegisterSSR ssr;
        uint8_t byte;
    } U = { SSR };

    U.ssr.B = 1;
    U.ssr._ = 1;
    push08(U.byte);
}

void cpu6502::InstrPLA( uint16_t src )
{
    AC = pop08();
}

void cpu6502::InstrPLP( uint16_t src )
{
    union {
        uint8_t byte;
        cpu6502RegisterSSR ssr;
    } U = { pop08() };

    U.ssr.B = SSR.B;
    U.ssr._ = SSR._;
    SSR = U.ssr;
}

void cpu6502::InstrSBC( uint16_t src )
{

}

void cpu6502::InstrSTA( uint16_t src )
{

}


