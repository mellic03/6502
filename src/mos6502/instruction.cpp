#include "mos6502.hpp"

void MOS6502::InstrUnimp()
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
uint8_t MOS6502::_N(uint16_t x)
{
    SSR.N = (x & (1 << 7)) ? 1 : 0;
    return (uint8_t)x;
}

uint8_t MOS6502::_NZ(uint16_t x)
{
    SSR.Z = (x == 0) ? 1 : 0;
    return _N(x);
}

uint8_t MOS6502::_NZC(uint16_t x)
{
    SSR.C = (x & (1 << 8)) ? 1 : 0;
    return _NZ(x);
}

uint8_t MOS6502::_NVZC(uint16_t x, uint8_t a, uint8_t b)
{
    SSR.V = ((a ^ x) & (b ^ x) & 0x80) ? 1 : 0;
    return _NZC(x);
}

#define SET_NZ(x, a, op, b)                    \
    uint16_t Tmp = uint16_t(a) op uint16_t(b); \
    SSR.N = (Tmp & (1 << 7)) ? 1 : 0;          \
    SSR.Z = (Tmp == 0) ? 1 : 0;

#define SET_NZC(x, a, op, b)                   \
    uint16_t Tmp = uint16_t(a) op uint16_t(b); \
    SSR.N = (Tmp & (1 << 7)) ? 1 : 0;          \
    SSR.Z = (Tmp == 0) ? 1 : 0;                \
    SSR.C = (Tmp & (1 << 8)) ? 1 : 0;

#define SET_NVZC(x, a, op, b)                       \
    uint16_t Tmp = uint16_t(a) op uint16_t(b);      \
    SSR.N = (Tmp & (1 << 7)) ? 1 : 0;               \
    SSR.V = ((a ^ Tmp) & (b ^ Tmp) & 0x80) ? 1 : 0; \
    SSR.Z = (Tmp == 0) ? 1 : 0;                     \
    SSR.C = (Tmp & (1 << 8)) ? 1 : 0;

void MOS6502::_IntPush()
{
    PC += 1;
    push08(PC_hi);
    push08(PC_lo);
    push08(SSR.byte);
    SSR.I = 1;
}

void MOS6502::_IntJump(uint16_t addr)
{
    uint16_t lo = rdbus(addr + 0);
    uint16_t hi = rdbus(addr + 1);
    PC = (hi << 8) | lo;
}

void MOS6502::_InstrNMI()
{
    _IntPush();
    SSR.B = 0;

    uint16_t lo = rdbus(0xFFFE);
    uint16_t hi = rdbus(0xFFFF);
    PC = (hi << 8) | lo;
}

void MOS6502::_InstrIRQ()
{
    if (SSR.I == 1)
    {
        return;
    }

    _IntPush();
    _IntJump(0xFFFA);
}

void MOS6502::_InstrADC(uint8_t b)
{
    static constexpr uint16_t BIT_7 = (1 << 7);
    static constexpr uint16_t BIT_8 = (1 << 8);

    uint16_t a = (uint16_t)AC;
    // uint16_t x = a + b + SSR.C;
    SET_NVZC(AC, a, +, b);
    // AC = _NVZC(x, a, b);
}

void MOS6502::InstrADC()
{
    _InstrADC(rdbus(mOpAddr));
}

void MOS6502::InstrAND()
{
    uint16_t x = AC & rdbus(mOpAddr);
    AC = _NZ(x);
}

void MOS6502::InstrASL()
{
    if (mOpAC)
    {
        uint16_t x = (uint16_t)AC;
        AC = _NZC(x << 1);
    }
    else
    {
        uint16_t x = (uint16_t)rdbus(mOpAddr);
        wtbus(mOpAddr, _NZC(x << 1));
    }
}

void MOS6502::InstrBCC()
{
    if (SSR.C == 0)
        PC = mOpAddr;
}

void MOS6502::InstrBCS()
{
    if (SSR.C == 1)
        PC = mOpAddr;
}

void MOS6502::InstrBEQ()
{
    if (SSR.Z == 1)
        PC = mOpAddr;
}

void MOS6502::InstrBIT()
{
    MOS_6502::RegSSR tmp = {rdbus(mOpAddr)};
    SSR.N = tmp.N;
    SSR.V = tmp.V;
}

void MOS6502::InstrBMI()
{
    if (SSR.N == 1)
        PC = mOpAddr;
}

void MOS6502::InstrBNE()
{
    if (SSR.Z == 0)
        PC = mOpAddr;
}

void MOS6502::InstrBPL()
{
    if (SSR.N == 0)
        PC = mOpAddr;
}

// BRK not affected by I
void MOS6502::InstrBRK()
{
    _IntPush();
    _IntJump(0xFFFA);
}

void MOS6502::InstrBVC()
{
    if (SSR.V == 0)
        PC = mOpAddr;
}

void MOS6502::InstrBVS()
{
    if (SSR.V == 1)
        PC = mOpAddr;
}

void MOS6502::InstrCLC() { SSR.C = 0; }
void MOS6502::InstrCLD() { SSR.D = 0; }
void MOS6502::InstrCLI() { SSR.I = 0; }
void MOS6502::InstrCLV() { SSR.V = 0; }

void MOS6502::InstrCMP()
{
    uint16_t x = (uint16_t)AC - rdbus(mOpAddr);
    _NZC(x);
}

void MOS6502::InstrCPX()
{
    uint16_t x = (uint16_t)XR - rdbus(mOpAddr);
    _NZC(x);
}

void MOS6502::InstrCPY()
{
    uint16_t x = (uint16_t)YR - rdbus(mOpAddr);
    _NZC(x);
}

void MOS6502::InstrDEC()
{
    uint16_t x = (uint16_t)rdbus(mOpAddr) - 1;
    wtbus(mOpAddr, _NZ(x));
}

void MOS6502::InstrDEX()
{
    uint16_t x = (uint16_t)XR - 1;
    XR = _NZ(x);
}

void MOS6502::InstrDEY()
{
    uint16_t x = (uint16_t)YR - 1;
    YR = _NZ(x);
}

void MOS6502::InstrEOR()
{
    uint16_t x = (uint16_t)AC ^ rdbus(mOpAddr);
    AC = _NZ(x);
}

void MOS6502::InstrINC()
{
    uint16_t x = (uint16_t)rdbus(mOpAddr) + 1;
    wtbus(mOpAddr, _NZ(x));
}

void MOS6502::InstrINX()
{
    XR = _NZ((uint16_t)XR + 1);
}

void MOS6502::InstrINY()
{
    YR = _NZ((uint16_t)YR + 1);
}

void MOS6502::InstrJMP()
{
    PC = mOpAddr;
}

void MOS6502::InstrJSR()
{
    push08(PC_hi);
    push08(PC_lo);
    PC = mOpAddr;
}

void MOS6502::InstrLDA()
{
    uint16_t x = (uint16_t)rdbus(mOpAddr);
    AC = _NZ(x);
}

void MOS6502::InstrLDX()
{
    uint8_t x = rdbus(mOpAddr);
    XR = _NZ(x);
}

void MOS6502::InstrLDY()
{
    uint8_t x = rdbus(mOpAddr);
    YR = _NZ(x);
}

void MOS6502::InstrLSR()
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

void MOS6502::InstrNOP()
{
}

void MOS6502::InstrORA()
{
    uint8_t x = rdbus(mOpAddr);
    _NZ(AC | x);
}

void MOS6502::InstrPHA()
{
    push08(AC);
}

void MOS6502::InstrPHP()
{
    SSR.B = 1;
    push08(SSR.byte);
}

void MOS6502::InstrPLA()
{
    AC = pop08();
    _NZ(AC);
}

void MOS6502::InstrPLP()
{
    SSR.byte = pop08();
}

void MOS6502::InstrROL()
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

void MOS6502::InstrROR()
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

void MOS6502::InstrRTI()
{
    SSR.byte = pop08();
    SSR.B = 0;

    PC_lo = pop08();
    PC_hi = pop08();
}

void MOS6502::InstrRTS()
{
    PC_lo = pop08();
    PC_hi = pop08();
}

void MOS6502::InstrSBC()
{
    uint8_t onecomp = ~(rdbus(mOpAddr));
    uint8_t twocomp = onecomp + 1;
    _InstrADC(twocomp);
}

void MOS6502::InstrSEC() { SSR.C = 1; }
void MOS6502::InstrSED() { SSR.D = 1; }
void MOS6502::InstrSEI() { SSR.I = 1; }
void MOS6502::InstrSTA() { wtbus(mOpAddr, AC); }
void MOS6502::InstrSTX() { wtbus(mOpAddr, XR); }
void MOS6502::InstrSTY() { wtbus(mOpAddr, YR); }
void MOS6502::InstrTAX() { XR = _NZ(AC); }
void MOS6502::InstrTAY() { YR = _NZ(AC); }
void MOS6502::InstrTSX() { XR = _NZ(SP); }
void MOS6502::InstrTXA() { AC = _NZ(XR); }
void MOS6502::InstrTXS() { SP = XR; }
void MOS6502::InstrTYA() { YR = _NZ(AC); }
void MOS6502::InstrWAI() { mSigCurr.wai = 1; }
