#include "6502.hpp"

// Helpers
uint8_t  cpu6502::fetch8()  { return mBus.read(PC++); }
uint16_t cpu6502::fetch16() { uint16_t lo = fetch8(); uint16_t hi = fetch8(); return (hi<<8) | lo; }

// --- Addressing modes ---

uint8_t* cpu6502::LoadACC()  { return &AC; }

uint8_t* cpu6502::LoadIMM()  { return &mBus[PC++]; }

// Absolute
uint8_t* cpu6502::LoadABS()
{
    uint16_t addr = fetch16();
    return &mBus[addr];
}
uint8_t* cpu6502::LoadABSX() {
    uint16_t base = fetch16();
    return &mBus[(base + XR) & 0xFFFF];
}
uint8_t* cpu6502::LoadABSY() {
    uint16_t base = fetch16();
    return &mBus[(base + YR) & 0xFFFF];
}

// Zero page (wrap at 0x00FF)
uint8_t* cpu6502::LoadZPG()  { return &mBus[fetch8()]; }
uint8_t* cpu6502::LoadZPGX() { return &mBus[(uint8_t)(fetch8() + XR)]; }
uint8_t* cpu6502::LoadZPGY() { return &mBus[(uint8_t)(fetch8() + YR)]; }

// Relative (return ptr to the signed offset byte)
uint8_t* cpu6502::LoadREL()  { return &mBus[PC++]; }

// (addr) — used by JMP only; emulate the 6502 page-wrap bug
uint8_t* cpu6502::LoadIND() {
    uint16_t ptr = fetch16();
    uint16_t lo_addr = ptr;
    uint16_t hi_addr = (ptr & 0xFF00) | ((ptr + 1) & 0x00FF); // 6502 bug
    uint16_t addr = (mBus.read(hi_addr) << 8) | mBus.read(lo_addr);
    return &mBus[addr];
}

// (zp,X)
uint8_t* cpu6502::LoadINDX() {
    uint8_t zp = fetch8();
    uint8_t p  = (uint8_t)(zp + XR);
    uint16_t addr = (mBus.read((uint8_t)(p + 1)) << 8) | mBus.read(p);
    return &mBus[addr];
}

// (zp),Y
uint8_t* cpu6502::LoadINDY() {
    uint8_t zp = fetch8();
    uint16_t base = (mBus.read((uint8_t)(zp + 1)) << 8) | mBus.read(zp);
    return &mBus[(base + YR) & 0xFFFF];
}
