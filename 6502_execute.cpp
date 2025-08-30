#include "6502.hpp"


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

}

void cpu6502::InstrBCS( uint16_t src )
{

}

void cpu6502::InstrBEQ( uint16_t src )
{

}

void cpu6502::InstrBIT( uint16_t src )
{

}

void cpu6502::InstrBNI( uint16_t src )
{

}

void cpu6502::InstrBNE( uint16_t src )
{

}

void cpu6502::InstrBPL( uint16_t src )
{

}

void cpu6502::InstrBRK( uint16_t src )
{

}

void cpu6502::InstrBVC( uint16_t src )
{

}

void cpu6502::InstrBVS( uint16_t src )
{

}




void cpu6502::InstrCMP( uint16_t src )
{

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

void cpu6502::InstrSBC( uint16_t src )
{

}

void cpu6502::InstrSTA( uint16_t src )
{

}


