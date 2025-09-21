#include <memu/hw/mostech/6502.hpp>


void m6502::InstrUnimp()
{
    mInvalidOp = 1;
}

/*
    Carry Flag:
        - C=0 when addition result is 0 to 255.
        - C=1 when addition result is greater than 255.
        - C=1 when subtraction result is 0 to 255.
        - C=0 when subtraction result is less than 0.
*/
uint8_t m6502::_N(uint8_t x)
{
    SSR.N = (x & (1 << 7)) ? 1 : 0;
    return x;
}

uint8_t m6502::_Z(uint8_t x)
{
    SSR.Z = (x == 0) ? 1 : 0;
    return x;
}

uint8_t m6502::_NZ(uint8_t x)
{
    _N(x); _Z(x);
    return x;
}

uint8_t m6502::_NZC(uint8_t x)
{
    SSR.C = (x & (1 << 8)) ? 1 : 0;
    return _NZ(x);
}

uint8_t m6502::_NVZC(uint8_t x, uint8_t a, uint8_t b)
{
    SSR.V = ((~(a^b)) & (a^x)) & 0x80;
    return _NZC(x);
}

/*
template <typename T, typename U>
T &cast_assign( T &a, U b ) { a=T(b); return a; }

#define SET_NZ(Dst, a, op, b)       \
    int32_t ValA = int32_t(a);      \
    int32_t ValB = int32_t(b);      \
    int32_t AopB = ValA op ValB;    \
    SSR.N = AopB < 0;               \
    SSR.Z = AopB == 0;              \
    Dst = cast_assign(Dst, AopB);

#define SET_NZC(Dst, a, op, b)          \
    int32_t ValA = int32_t(a);          \
    int32_t ValB = int32_t(b);          \
    int32_t AopB = ValA op ValB;        \
    SSR.N = AopB < 0;                   \
    SSR.Z = AopB == 0;                  \
    SSR.C = (AopB & (1 << 8)) ? 1 : 0;  \
    Dst = cast_assign(Dst, AopB);

#define SET_NVZC(Dst, a, op, b)         \
    int32_t ValA = int32_t(a);          \
    int32_t ValB = int32_t(b);          \
    int32_t AopB = ValA op ValB;        \
    SSR.N = AopB < 0;                   \
    SSR.V = !(-128<=AopB && AopB<=127); \
    SSR.Z = AopB == 0;                  \
    SSR.C = (AopB & (1 << 8)) ? 1 : 0;  \
    Dst = cast_assign(Dst, AopB);
*/

void m6502::_IntPush()
{
    PC += 1;
    push16(PC);
    push08(SSR.as_byte);
    SSR.I = 1;
}

void m6502::_IntJump(uint16_t addr)
{
    PC_lo = rdbus(addr+0);
    PC_hi = rdbus(addr+1);
}

void m6502::_InstrADC(uint8_t b)
{
    AC = _NVZC(AC+b, AC, b);
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
    static auto tmp = SSR;
    tmp.as_byte = rdbus(mOpAddr);
    SSR.N = tmp.N;
    SSR.V = tmp.V;
    _Z(AC & tmp.as_byte);
}

void m6502::InstrBMI() { if (SSR.N==1) PC = mOpAddr; }
void m6502::InstrBNE() { if (SSR.Z==0) PC = mOpAddr; }
void m6502::InstrBPL() { if (SSR.N==0) PC = mOpAddr; }

// BRK not affected by I
void m6502::InstrBRK()
{
    push16(PC + 1);
    push08(SSR.as_byte);
    SSR.I = 1;
    _IntJump(0xFFFA);
}

void m6502::InstrBVC() { if (SSR.V==0) PC = mOpAddr; }
void m6502::InstrBVS() { if (SSR.V==1) PC = mOpAddr; }

void m6502::InstrCLC() { SSR.C = 0; }
void m6502::InstrCLD() { SSR.D = 0; }
void m6502::InstrCLI() { SSR.I = 0; }
void m6502::InstrCLV() { SSR.V = 0; }

void m6502::InstrCMP() { _NZC(AC - rdbus(mOpAddr)); }
void m6502::InstrCPX() { _NZC(AC - rdbus(mOpAddr)); }
void m6502::InstrCPY() { _NZC(YR - rdbus(mOpAddr)); }
void m6502::InstrDEC() { wtbus(mOpAddr, _NZ(rdbus(mOpAddr))); }
void m6502::InstrDEX() { XR = _NZ(XR - 1); }
void m6502::InstrDEY() { YR = _NZ(YR - 1); }
void m6502::InstrEOR() { AC = _NZ(AC ^ rdbus(mOpAddr)); }
void m6502::InstrINC() { wtbus(mOpAddr, _NZ(rdbus(mOpAddr) + 1)); }
void m6502::InstrINX() { XR = _NZ(XR + 1); }
void m6502::InstrINY() { YR = _NZ(YR + 1); }
void m6502::InstrJMP() { PC = mOpAddr; }
void m6502::InstrJSR() { push16(PC); PC = mOpAddr; }
void m6502::InstrLDA() { AC = _NZ(rdbus(mOpAddr)); }
void m6502::InstrLDX() { XR = _NZ(rdbus(mOpAddr)); }
void m6502::InstrLDY() { YR = _NZ(rdbus(mOpAddr)); }

void m6502::InstrLSR()
{
    if (mOpAC)
    {
        uint16_t x = AC;
        AC = _NZC(x >> 1);
    }

    else
    {
        uint16_t x = rdbus(mOpAddr);
        wtbus(mOpAddr, _NZC(x >> 1));
    }

    SSR.N = 0;
}

void m6502::InstrNOP() {   }
void m6502::InstrORA() { AC =_NZ(AC | rdbus(mOpAddr)); }
void m6502::InstrPHA() { push08(AC); }
void m6502::InstrPHP() { SSR.B = 1;  push08(SSR.as_byte); }
void m6502::InstrPLA() { AC = _NZ(pop08()); }
void m6502::InstrPLP() { SSR.as_byte = pop08(); }

void m6502::InstrROL()
{
    if (mOpAC)
    {
        uint16_t v = AC;
        uint16_t b = (v & (1 << 7)) ? 1 : 0;
        uint16_t x = (v << 1) | b;
        AC = _NZC(x);
    }
    else
    {
        uint16_t v = rdbus(mOpAddr);
        uint16_t b = (v & (1 << 7)) ? 1 : 0;
        uint16_t x = (v << 1) | b;
        wtbus(mOpAddr, _NZC(x));
    }
}

void m6502::InstrROR()
{
    if (mOpAC)
    {
        uint16_t v = AC;
        uint16_t b = (v & 0x01) ? 1 : 0;
        uint16_t x = (b << 7) | (v >> 1);
        AC = _NZC(x);
    }
    else
    {
        uint16_t v = rdbus(mOpAddr);
        uint16_t b = (v & 0x01) ? 1 : 0;
        uint16_t x = (b << 7) | (v >> 1);
        wtbus(mOpAddr, _NZC(x));
    }
}

void m6502::InstrRTI()
{
    SSR.as_byte = pop08();
    SSR.B = 0;
    PC = pop16();
}

void m6502::InstrRTS() { PC = pop16(); }
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
void m6502::InstrTYA() { YR = _NZ(AC); }
