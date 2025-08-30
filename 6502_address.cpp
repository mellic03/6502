#include "6502.hpp"



uint16_t cpu6502::_read16()
{
    uint8_t lo = mBus[PC+0];
    uint8_t hi = mBus[PC+1];
    PC += 2;
    return ((uint16_t)hi << 8) + (uint16_t)lo;
}

uint8_t *cpu6502::LoadACC()
{
    return &AC;
}

uint8_t *cpu6502::LoadABS()
{
    uint16_t addr = _read16();
    return &mBus[addr];
}

uint8_t *cpu6502::LoadABSX()
{
    uint16_t addr = _read16();
    return &mBus[addr + (uint16_t)XR];
}

uint8_t *cpu6502::LoadABSY()
{
    uint16_t addr = _read16();
    return &mBus[addr + (uint16_t)YR];
}

uint8_t *cpu6502::LoadIMM()
{
    static uint8_t byte;
    byte = mBus[PC];
    PC += 1;
    return &byte;
}

uint8_t *cpu6502::LoadIMP()
{
    PC += 1;
    return 0;
}

uint8_t *cpu6502::LoadIND()
{
    uint16_t addr = _read16();
    uint16_t lo = mBus[addr+0];
    uint16_t hi = mBus[addr+1];
    return &mBus[(hi<<8) + lo];
}

uint8_t *cpu6502::LoadINDX()
{
    uint16_t addr = _read16();
    uint16_t lo = (mBus[addr+0] + XR) & 0x00FF;
    uint16_t hi = (lo + 1) & 0x00FF;
    return &mBus[(hi << 8) + lo];
}

uint8_t *cpu6502::LoadINDY()
{
    uint16_t addr = _read16();
    uint16_t lo = (mBus[addr+0]) & 0x00FF;
    uint16_t hi = (lo + 1) & 0x00FF;
    return &mBus[(hi << 8) + lo + (uint16_t)YR];
}

uint8_t *cpu6502::LoadREL()
{
    union {
        uint8_t u08;
        int8_t  i08;
    } U = { mBus[PC] };

    uint16_t addr = PC + U.i08;
    PC += 1;

    return &mBus[addr];
}

uint8_t *cpu6502::LoadZPG()
{
    uint16_t lo = mBus[PC];
    uint16_t hi = 0x00;
    PC += 1;
    return &mBus[(hi<<8) + lo];
}

uint8_t *cpu6502::LoadZPGX()
{
    uint8_t lo = mBus[PC];
    uint8_t hi = 0x00;
    PC += 1;
    return &mBus[((uint16_t)hi << 8) + (uint16_t)lo + (uint16_t)XR];
}

uint8_t *cpu6502::LoadZPGY()
{
    uint8_t lo = mBus[PC];
    uint8_t hi = 0x00;
    PC += 1;
    return &mBus[((uint16_t)hi << 8) + (uint16_t)lo + (uint16_t)YR];
}

