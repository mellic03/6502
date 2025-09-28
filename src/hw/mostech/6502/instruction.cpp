#include <memu/hw/mostech/6502.hpp>
#include <memu/addrspace.hpp>


void m6502::InstrUnimp()
{
    mInvalidOp = 1;
}

uint8_t m6502::_N(uword x)
{
    SSR.N = (x & (1 << 7)) ? 1 : 0;
    return x;
}

uint8_t m6502::_V(uword x, uint8_t a, uint8_t b)
{
    SSR.V = ((~(a^b)) & (a^x)) & 0x80;
    return x;
}

uint8_t m6502::_Z(uword x)
{
    SSR.Z = (x & 0xFF) ? 0 : 1;
    return x;
}

uint8_t m6502::_NZ(uword x)
{
    _N(x); _Z(x);
    return x;
}

uint8_t m6502::_NZC(uword x)
{
    SSR.C = (0<=x && x<=255) ? 0 : 1;
    // SSR.C = ((uword)x & (1 << 8)) ? 1 : 0;
    _N(x); _Z(x);
    return x;
}

uint8_t m6502::_NVZC(uword x, uint8_t a, uint8_t b)
{
    SSR.V = ((~(a^b)) & (a^x)) & 0x80;
    _NZC(x);
    return x;
}




#include <cstdio>


/*
    Important distinction
    - Stack copy of SSR: is a snapshot taken before the interrupt,
      with B adjusted according to source (IRQ vs BRK).

    - Real SSR: gets I=1 after the push, so further IRQs are
      masked until cleared.
*/
void m6502::_IntPush( ubyte B, ubyte U )
{
    static auto stat = SSR;

    push16(PC);

    stat = SSR;
    stat.B = B;
    stat.U = U;
    push08(stat.byte);

    SSR.I = 1;
}

void m6502::_IntJump( uword addr )
{
    PC = rdbusw(addr);
}


void m6502::_NMI()
{
    // printf("\t\t NMI  PC:%04X\n", PC);
    mWaiting = false;
    _IntPush(0, 1);
    _IntJump(0xFFFA);
}

void m6502::_RES()
{
    // printf("\t\t RES  PC:%04X\n", PC);
    mWaiting = false;
    this->reset();
}

void m6502::_IRQ()
{
    mWaiting = false;

    if (SSR.I == 1)
    {
        return;
    }

    _IntPush(0);
    _IntJump(0xFFFE);
}

void m6502::_BRK()
{
    _IntPush(1);
    _IntJump(0xFFFE);
}





void m6502::_InstrADC( uint8_t arg )
{
    uword sum = AC + arg + (SSR.C ? 1 : 0);
    SSR.V = ((AC ^ sum) & (arg ^ sum) & 0x80) ? 1 : 0;
    AC    = _NZC(AC + arg + SSR.C);
}

ubyte m6502::_InstrReadAccMaybe()
{
    return (mOpAC) ? AC : rdbus(mOpAddr);
}

void m6502::_InstrWriteAccMaybe( ubyte value )
{
    if (mOpAC) AC = value;
    else wtbus(mOpAddr, value);
}




void m6502::InstrADC() { _InstrADC(rdbus(mOpAddr)); }
void m6502::InstrAND() { AC = _NZ(AC & rdbus(mOpAddr)); }

void m6502::InstrASL()
{
    if (mOpAC) AC = _NZC(AC << 1);
    else       wtbus(mOpAddr, _NZC(rdbus(mOpAddr) << 1));
}

void m6502::InstrBCC() { if (SSR.C==0) PC = mOpAddr; }
void m6502::InstrBCS() { if (SSR.C==1) PC = mOpAddr; }
void m6502::InstrBEQ() { if (SSR.Z==1) PC = mOpAddr; }

void m6502::InstrBIT()
{
    uint8_t value = rdbus(mOpAddr);
    SSR.N = (value & (1<<7)) ? 1 : 0;
    SSR.V = (value & (1<<6)) ? 1 : 0;
    SSR.Z = ((value & AC) == 0) ? 1 : 0;
}

void m6502::InstrBMI() { if (SSR.N==1) PC = mOpAddr; }
void m6502::InstrBNE() { if (SSR.Z==0) PC = mOpAddr; }
void m6502::InstrBPL() { if (SSR.N==0) PC = mOpAddr; }
void m6502::InstrBRK() { _BRK(); }
void m6502::InstrBVC() { if (SSR.V==0) PC = mOpAddr; }
void m6502::InstrBVS() { if (SSR.V==1) PC = mOpAddr; }

void m6502::InstrCLC() { SSR.C = 0; }
void m6502::InstrCLD() { SSR.D = 0; }
void m6502::InstrCLI() { SSR.I = 0; }
void m6502::InstrCLV() { SSR.V = 0; }


#define instr_cmp \
    if (rg < op) { \
        SSR.N = ((rg-op) & 128) ? 1 : 0; \
        SSR.Z = 0; \
        SSR.C = 0; \
    } else if (rg == op) { \
        SSR.N = 0; \
        SSR.Z = 1; \
        SSR.C = 1; \
    } else if (rg > op) { \
        SSR.N = ((rg-op) & 128) ? 1 : 0; \
        SSR.Z = 0; \
        SSR.C = 1; \
    }

void m6502::InstrCMP()
{
    ubyte rg = AC;
    ubyte op = rdbus(mOpAddr);
    instr_cmp
}

void m6502::InstrCPX()
{
    ubyte rg = XR;
    ubyte op = rdbus(mOpAddr);
    instr_cmp
}

void m6502::InstrCPY()
{
    ubyte rg = YR;
    ubyte op = rdbus(mOpAddr);
    instr_cmp
}



void m6502::InstrDEC() { wtbus(mOpAddr, _NZ(rdbus(mOpAddr)-1)); }
void m6502::InstrDEX() { XR = _NZ(XR-1); }
void m6502::InstrDEY() { YR = _NZ(YR-1); }
void m6502::InstrEOR() { AC = _NZ(AC ^ rdbus(mOpAddr)); }
void m6502::InstrINC() { wtbus(mOpAddr, _NZ(rdbus(mOpAddr)+1)); }
void m6502::InstrINX() { XR = _NZ(XR+1); }
void m6502::InstrINY() { YR = _NZ(YR+1); }
void m6502::InstrJMP() { PC = mOpAddr; }
void m6502::InstrJSR() { push16(PC-1); PC = mOpAddr; }
void m6502::InstrLDA() { AC = _NZ(rdbus(mOpAddr)); }
void m6502::InstrLDX() { XR = _NZ(rdbus(mOpAddr)); }
void m6502::InstrLDY() { YR = _NZ(rdbus(mOpAddr)); }

void m6502::InstrLSR()
{
    ubyte value = _InstrReadAccMaybe();
    _InstrWriteAccMaybe(_NZC(value >> 1));
    SSR.C = (value & 0b01) ? 1 : 0;
}

void m6502::InstrNOP() {   }
void m6502::InstrORA() { AC =_NZ(AC | rdbus(mOpAddr)); }
void m6502::InstrPHA() { push08(AC); }
void m6502::InstrPLA() { AC = _NZ(pop08()); }

void m6502::InstrPHP()
{
    auto P = SSR;
    P.B = 1;
    P.U = 1;
    push08(P.byte);
}

void m6502::InstrPLP()
{
    auto P = SSR;
    P.byte = pop08();
    P.B = SSR.B;
    P.U = SSR.U;
    SSR = P;
}


void m6502::InstrROL()
{
    ubyte C   = (SSR.C << 0);
    ubyte val = _InstrReadAccMaybe();
    ubyte res = (val << 1) | C;
    _InstrWriteAccMaybe(res);

    SSR.N = (res & (1<<7)) ? 1 : 0;
    SSR.Z = (res == 0) ? 1 : 0;
    SSR.C = (val & (1<<7)) ? 1 : 0;
}

void m6502::InstrROR()
{
    ubyte C   = (SSR.C << 7);
    ubyte val = _InstrReadAccMaybe();
    ubyte res = (val >> 1) | C;
    _InstrWriteAccMaybe(res);

    SSR.N = (res & (1<<7)) ? 1 : 0;
    SSR.Z = (res == 0) ? 1 : 0;
    SSR.C = (val & (1<<0)) ? 1 : 0;
}

void m6502::InstrRTI()
{
    auto P = SSR;
    P.byte = pop08();
    P.B = SSR.B;
    SSR = P;
    // SSR.B = 0;
    PC = pop16();
}

void m6502::InstrRTS() { PC = pop16()+1; }
void m6502::InstrSBC() { _InstrADC(~rdbus(mOpAddr)); }
void m6502::InstrSEC() { SSR.C = 1; }
void m6502::InstrSED() { SSR.D = 1; }
void m6502::InstrSEI() { SSR.I = 1; }
void m6502::InstrSTA() { wtbus(mOpAddr, AC); }
void m6502::InstrSTX() { wtbus(mOpAddr, XR); }
void m6502::InstrSTY() { wtbus(mOpAddr, YR); }
void m6502::InstrTAX() { XR = _NZ(AC); }
void m6502::InstrTAY() { YR = _NZ(AC); }
void m6502::InstrTSX() { XR = _NZ(SP); }
void m6502::InstrTXA() { AC = _NZ(XR); }
void m6502::InstrTXS() { SP = XR; }
void m6502::InstrTYA() { AC = _NZ(YR); }
