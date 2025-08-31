#include "6502.hpp"


uint8_t cpu6502::_adv08()
{
    return mBus[PC++];
}

uint16_t cpu6502::_adv16()
{
    uint16_t lo = mBus[PC++];
    uint16_t hi = mBus[PC++];
    return (hi << 8) | lo;
}

uint8_t *cpu6502::LoadACC()
{
    return &AC;
}

uint8_t *cpu6502::LoadABS()
{
    uint16_t addr = _adv16();
    return &mBus[addr];
}

uint8_t *cpu6502::LoadABSX()
{
    uint16_t addr = _adv16();
    return &mBus[addr + (uint16_t)XR];
}

uint8_t *cpu6502::LoadABSY()
{
    uint16_t addr = _adv16();
    return &mBus[addr + (uint16_t)YR];
}

uint8_t *cpu6502::LoadIMM()
{
    uint16_t addr = PC++;
    return &mBus[addr];
}

uint8_t *cpu6502::LoadIMP()
{
    // PC += 1;
    return 0;
}

uint8_t *cpu6502::LoadIND()
{
    uint16_t addr = _adv16();
    uint16_t lo = mBus[addr+0];
    uint16_t hi = mBus[addr+1];
    return &mBus[(hi<<8) | lo];
}

uint8_t *cpu6502::LoadINDX()
{
    uint16_t addr = _adv16();
    uint16_t lo   = mBus[(addr+XR+0) & 0x00FF];
    uint16_t hi   = mBus[(addr+XR+1) & 0x00FF];
    return &mBus[(hi << 8) | lo];
}

uint8_t *cpu6502::LoadINDY()
{
    uint16_t addr = _adv16();
    uint16_t lo   = mBus[(addr+0) & 0x00FF];
    uint16_t hi   = mBus[(addr+1) & 0x00FF];

    addr = (hi<<8) | lo;
    addr += YR;

    return &mBus[addr];
}

uint8_t *cpu6502::LoadREL()
{
    uint16_t addr = _adv16();
    if (addr & 0x80)
        addr |= 0xFF00;
    return &mBus[addr];
}

uint8_t *cpu6502::LoadZPG()
{
    uint16_t addr = uint16_t(_adv08()) & 0x00FF;
    return &mBus[addr];
}

uint8_t *cpu6502::LoadZPGX()
{
    uint16_t addr = uint16_t(_adv08() | XR) & 0x00FF;
    return &mBus[addr];
}

uint8_t *cpu6502::LoadZPGY()
{
    uint16_t addr = uint16_t(_adv08() | YR) & 0x00FF;
    return &mBus[addr];
}

