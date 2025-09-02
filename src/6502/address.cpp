#include "6502.hpp"


uint8_t cpu6502::_adv08()
{
    return mBus.read(PC++);
}

uint16_t cpu6502::_adv16()
{
    uint16_t lo = mBus.read(PC++);
    uint16_t hi = mBus.read(PC++);
    return (hi << 8) | lo;
}

uint8_t *cpu6502::LoadACC()
{
    return &AC;
}


uint8_t *cpu6502::LoadABS()
{
    PC += 2;
    return &mBus[PC-2];
}

uint8_t *cpu6502::LoadABSX()
{
    PC += 2;
    return &mBus[PC-2 + XR];
}

uint8_t *cpu6502::LoadABSY()
{
    PC += 2;
    return &mBus[PC-1 + YR];
}

uint8_t *cpu6502::LoadIMM()
{
    PC += 1;
    return &mBus[PC-1];
}

uint8_t *cpu6502::LoadIMP()
{
    static uint8_t dummy = 0;
    return &dummy;
}

uint8_t *cpu6502::LoadIND()
{
    PC += 2;
    uint16_t addr = mBus.read16(PC-2);
    uint16_t lo   = mBus.read(addr+0) & 0x00FF;
    uint16_t hi   = mBus.read(addr+1) & 0x00FF;
    return &mBus[(hi << 8) | lo];
}


/*
    (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
*/
uint8_t *cpu6502::LoadINDX()
{
    PC += 2;
    uint16_t addr = mBus.read16(PC-2);
    uint16_t lo   = mBus.read(addr+XR+0) & 0x00FF;
    uint16_t hi   = mBus.read(addr+XR+1) & 0x00FF;
    return &mBus[(hi << 8) | lo];
}


/*
    OPC ($LL),Y operand is zeropage address; effective address is
    word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
*/
uint8_t *cpu6502::LoadINDY()
{
    PC += 2;
    uint16_t addr = mBus.read16(PC-2);
    uint16_t lo   = mBus.read(addr+0) & 0x00FF;
    uint16_t hi   = mBus.read(addr+1) & 0x00FF;
    return &mBus[((hi << 8) | lo) + YR];
}

uint8_t *cpu6502::LoadREL()
{
    return (uint8_t*)&mBus[PC++];
}

uint8_t *cpu6502::LoadZPG()
{
    return &mBus[uint16_t(_adv08()) & 0x00FF];
}

uint8_t *cpu6502::LoadZPGX()
{
    return &mBus[uint16_t(_adv08() + XR) & 0x00FF];
}

uint8_t *cpu6502::LoadZPGY()
{
    return &mBus[uint16_t(_adv08() + YR) & 0x00FF];
}

