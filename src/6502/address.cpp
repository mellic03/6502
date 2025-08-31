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


#include <stdio.h>
uint8_t *cpu6502::LoadABS()
{
    // uint16_t addr = _adv16();
    // uint16_t 
    // printf("ABS 0x%02X\n", addr);
    // uint16_t addr = rdbus16(PC);
    // PC += 2;
    PC += 2;
    return &mBus[PC-2];
}

uint8_t *cpu6502::LoadABSX()
{
    // uint16_t addr = _adv16();
    // return &mBus[addr + (uint16_t)XR];
    PC += 2;
    return &mBus[PC-2 + XR];
}

uint8_t *cpu6502::LoadABSY()
{
    // uint16_t addr = _adv16();
    // return &mBus[addr + (uint16_t)YR];
    PC += 2;
    return &mBus[PC-2 + YR];
}

uint8_t *cpu6502::LoadIMM()
{
    uint8_t *ptr = &mBus[PC++];
    return ptr;
}

uint8_t *cpu6502::LoadIMP()
{
    return 0;
}

uint8_t *cpu6502::LoadIND()
{
    PC += 2;
    uint16_t addr = rdbus16(PC); 
    // uint16_t addr = _adv16();
    uint16_t lo   = mBus[addr+0];
    uint16_t hi   = mBus[addr+1];
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
    static uint16_t addr = 0;

    addr = PC + mBus[PC++] + 1;
    return (uint8_t*)(&addr);


    // uint16_t addr = PC + rdbus16(PC);
    // uint16_t addr = _adv16();
    // if (addr & 0x80)
        // addr |= 0xFF00;
    // return &mBus[addr];
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

