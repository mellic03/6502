#include <memu/hw/mostech/6502.hpp>
#include <memu/addrspace.hpp>
#include <memu/pinout.hpp>
#include <stdio.h>
#include <string.h>


// #define M6502_LOGGING

#ifdef M6502_LOGGING
    #define M605_LOG printf
#else
    #define M605_LOG(...)
#endif


void m6502::_fetch()
{
    M605_LOG("%04X %02X  ", PC, rdbus(PC));

    mOpAC   = false;
    mCurrOp = rdbus(PC++);
}

void m6502::_decode()
{
    mCurrInstr = &mFtab[mCurrOp];
    M605_LOG("%s    ", mCurrInstr->label);
}


void m6502::_execute()
{
    (this->*mCurrInstr->fA)();
    M605_LOG("A:%02X X:%02X Y:%02X P:%02X SP:%02X  ", AC, XR, YR, SSR.byte, SP);
    M605_LOG("\n");
    (this->*mCurrInstr->fE)();

    mClock += mCurrInstr->cycles;
    mOpCount += 1;
}

    

void m6502::tick()
{
    using namespace memu;

    if (ioRead(ioNMI))
    {
        _NMI();
        ioWrite(ioNMI, 0);
    }

    else if (ioRead(ioRES) == 0)
    {
        this->reset();
    }

    else if (ioRead(ioIRQ) == 0)
    {
        _IRQ();
    }

    if (mWaiting)
    {
        return;
    }

    _fetch();
    _decode();
    _execute();

    if (mInvalidOp)
    {
        printf("Invalid opcode (0x%02X)\n", mCurrOp);
    }
}


void m6502::reset()
{
    AC  = 0x00;
    XR  = 0x00;
    YR  = 0x99;
    SP  = 0xFD;
    PC  = 0xFFFC;
    SSR = {0b00100100};

    mWaiting   = false;
    mInvalidOp = 0;
    mCurrOp    = 0;
    mClock     = 0;
    mOpCount   = 0;

    _RES();
}

void m6502::push08( uint8_t byte )
{
    wtbus(0x0100 + --SP, byte);
}

void m6502::push16( uint16_t word )
{
    push08((uint8_t)(word >> 8));      // hi
    push08((uint8_t)(word & 0xFF));  // lo
}

uint8_t m6502::pop08()
{
    return rdbus(0x0100 + SP++);
}

uint16_t m6502::pop16()
{
    uint8_t lo = pop08();
    uint8_t hi = pop08();
    return ((uint16_t)hi << 8) | (uint16_t)lo;
}


m6502::m6502( memu::AddrSpace &bus )
:   HwModule(bus),
    BaseHw(),
    mWaiting(false),
    mInvalidOp(0),
    mCurrOp(0),
    mCurrClock(0),
    mPrevClock(0),
    mOpCount(0)
{
    using X = m6502;

    mFtab = new Inst[256];
    for (int i=0; i<256; i++)
    {
        mFtab[i] = Inst("Unimp", &X::InstrUnimp, &X::LoadACC, 1);
    }

    mFtab[0x69] = Inst("ADC", &X::InstrADC, &X::LoadIMM,  2);
    mFtab[0x65] = Inst("ADC", &X::InstrADC, &X::LoadZPG,  3);
    mFtab[0x75] = Inst("ADC", &X::InstrADC, &X::LoadZPGX, 4);
    mFtab[0x6D] = Inst("ADC", &X::InstrADC, &X::LoadABS,  4);
    mFtab[0x7D] = Inst("ADC", &X::InstrADC, &X::LoadABSX, 4);
    mFtab[0x79] = Inst("ADC", &X::InstrADC, &X::LoadABSY, 4);
    mFtab[0x61] = Inst("ADC", &X::InstrADC, &X::LoadINDX, 6);
    mFtab[0x71] = Inst("ADC", &X::InstrADC, &X::LoadINDY, 5);

    mFtab[0x29] = Inst("AND", &X::InstrAND, &X::LoadIMM,  2);
    mFtab[0x25] = Inst("AND", &X::InstrAND, &X::LoadZPG,  3);
    mFtab[0x35] = Inst("AND", &X::InstrAND, &X::LoadZPGX, 4);
    mFtab[0x2D] = Inst("AND", &X::InstrAND, &X::LoadABS,  4);
    mFtab[0x3D] = Inst("AND", &X::InstrAND, &X::LoadABSX, 4);
    mFtab[0x39] = Inst("AND", &X::InstrAND, &X::LoadABSY, 4);
    mFtab[0x21] = Inst("AND", &X::InstrAND, &X::LoadINDX, 6);
    mFtab[0x31] = Inst("AND", &X::InstrAND, &X::LoadINDY, 5);

    mFtab[0x0A] = Inst("ASL", &X::InstrASL, &X::LoadACC,  2);
    mFtab[0x06] = Inst("ASL", &X::InstrASL, &X::LoadZPG,  5);
    mFtab[0x16] = Inst("ASL", &X::InstrASL, &X::LoadZPGX, 6);
    mFtab[0x0E] = Inst("ASL", &X::InstrASL, &X::LoadABS,  6);
    mFtab[0x1E] = Inst("ASL", &X::InstrASL, &X::LoadABSX, 7);

    mFtab[0x90] = Inst("BCC", &X::InstrBCC, &X::LoadREL, 2);
    mFtab[0xB0] = Inst("BCS", &X::InstrBCS, &X::LoadREL, 2);
    mFtab[0xF0] = Inst("BEQ", &X::InstrBEQ, &X::LoadREL, 2);
    mFtab[0x24] = Inst("BIT", &X::InstrBIT, &X::LoadZPG, 3);
    mFtab[0x2C] = Inst("BIT", &X::InstrBIT, &X::LoadABS, 4);
    mFtab[0x30] = Inst("BMI", &X::InstrBMI, &X::LoadREL, 2);
    mFtab[0xD0] = Inst("BNE", &X::InstrBNE, &X::LoadREL, 2);
    mFtab[0x10] = Inst("BPL", &X::InstrBPL, &X::LoadREL, 2);
    mFtab[0x00] = Inst("BRK", &X::InstrBRK, &X::LoadIMP, 7);
    mFtab[0x50] = Inst("BVC", &X::InstrBVC, &X::LoadREL, 2);
    mFtab[0x70] = Inst("BVS", &X::InstrBVS, &X::LoadREL, 2);

    mFtab[0x18] = Inst("CLC", &X::InstrCLC, &X::LoadIMP, 2);
    mFtab[0xD8] = Inst("CLD", &X::InstrCLD, &X::LoadIMP, 2);
    mFtab[0x58] = Inst("CLI", &X::InstrCLI, &X::LoadIMP, 2);
    mFtab[0xB8] = Inst("CLV", &X::InstrCLV, &X::LoadIMP, 2);

    mFtab[0xC9] = Inst("CMP", &X::InstrCMP, &X::LoadIMM,  2);
    mFtab[0xC5] = Inst("CMP", &X::InstrCMP, &X::LoadZPG,  3);
    mFtab[0xD5] = Inst("CMP", &X::InstrCMP, &X::LoadZPGX, 4);
    mFtab[0xCD] = Inst("CMP", &X::InstrCMP, &X::LoadABS,  4);
    mFtab[0xDD] = Inst("CMP", &X::InstrCMP, &X::LoadABSX, 4);
    mFtab[0xD9] = Inst("CMP", &X::InstrCMP, &X::LoadABSY, 4);
    mFtab[0xC1] = Inst("CMP", &X::InstrCMP, &X::LoadINDX, 6);
    mFtab[0xD1] = Inst("CMP", &X::InstrCMP, &X::LoadINDY, 5);

    mFtab[0xE0] = Inst("CPX", &X::InstrCPX, &X::LoadIMM, 2);
    mFtab[0xE4] = Inst("CPX", &X::InstrCPX, &X::LoadZPG, 3);
    mFtab[0xEC] = Inst("CPX", &X::InstrCPX, &X::LoadABS, 4);
    mFtab[0xC0] = Inst("CPY", &X::InstrCPY, &X::LoadIMM, 2);
    mFtab[0xC4] = Inst("CPY", &X::InstrCPY, &X::LoadZPG, 3);
    mFtab[0xCC] = Inst("CPY", &X::InstrCPY, &X::LoadABS, 4);

    mFtab[0xC6] = Inst("DEC",  &X::InstrDEC, &X::LoadZPG,  5);
    mFtab[0xD6] = Inst("DEC",  &X::InstrDEC, &X::LoadZPGX, 6);
    mFtab[0xCE] = Inst("DEC",  &X::InstrDEC, &X::LoadABS,  6);
    mFtab[0xDE] = Inst("DEC",  &X::InstrDEC, &X::LoadABSX, 7);
    mFtab[0xCA] = Inst("DEX",  &X::InstrDEX, &X::LoadIMP,  2);
    mFtab[0x88] = Inst("DEY",  &X::InstrDEY, &X::LoadIMP,  2);

    mFtab[0x49] = Inst("EOR", &X::InstrEOR, &X::LoadIMM,  2);
    mFtab[0x45] = Inst("EOR", &X::InstrEOR, &X::LoadZPG,  3);
    mFtab[0x55] = Inst("EOR", &X::InstrEOR, &X::LoadZPGX, 4);
    mFtab[0x4D] = Inst("EOR", &X::InstrEOR, &X::LoadABS,  4);
    mFtab[0x5D] = Inst("EOR", &X::InstrEOR, &X::LoadABSX, 4);
    mFtab[0x59] = Inst("EOR", &X::InstrEOR, &X::LoadABSY, 4);
    mFtab[0x41] = Inst("EOR", &X::InstrEOR, &X::LoadINDX, 6);
    mFtab[0x51] = Inst("EOR", &X::InstrEOR, &X::LoadINDY, 5);

    mFtab[0xE6] = Inst("INC",  &X::InstrINC, &X::LoadZPG,  5);
    mFtab[0xF6] = Inst("INC",  &X::InstrINC, &X::LoadZPGX, 6);
    mFtab[0xEE] = Inst("INC",  &X::InstrINC, &X::LoadABS,  6);
    mFtab[0xFE] = Inst("INC",  &X::InstrINC, &X::LoadABSX, 7);
    mFtab[0xE8] = Inst("INX",  &X::InstrINX, &X::LoadIMP,  2);
    mFtab[0xC8] = Inst("INY",  &X::InstrINY, &X::LoadIMP,  2);

    mFtab[0x4C] = Inst("JMP",  &X::InstrJMP, &X::LoadABS, 3);
    mFtab[0x6C] = Inst("JMP",  &X::InstrJMP, &X::LoadIND, 5);
    mFtab[0x20] = Inst("JSR",  &X::InstrJSR, &X::LoadABS, 6);

    mFtab[0xA9] = Inst("LDA", &X::InstrLDA, &X::LoadIMM,  2);
    mFtab[0xA5] = Inst("LDA", &X::InstrLDA, &X::LoadZPG,  3);
    mFtab[0xB5] = Inst("LDA", &X::InstrLDA, &X::LoadZPGX, 4);
    mFtab[0xAD] = Inst("LDA", &X::InstrLDA, &X::LoadABS,  4);
    mFtab[0xBD] = Inst("LDA", &X::InstrLDA, &X::LoadABSX, 4);
    mFtab[0xB9] = Inst("LDA", &X::InstrLDA, &X::LoadABSY, 4);
    mFtab[0xA1] = Inst("LDA", &X::InstrLDA, &X::LoadINDX, 6);
    mFtab[0xB1] = Inst("LDA", &X::InstrLDA, &X::LoadINDY, 5);

    mFtab[0xA2] = Inst("LDX", &X::InstrLDX, &X::LoadIMM,  2);
    mFtab[0xA6] = Inst("LDX", &X::InstrLDX, &X::LoadZPG,  3);
    mFtab[0xB6] = Inst("LDX", &X::InstrLDX, &X::LoadZPGY, 4);
    mFtab[0xAE] = Inst("LDX", &X::InstrLDX, &X::LoadABS,  4);
    mFtab[0xBE] = Inst("LDX", &X::InstrLDX, &X::LoadABSY, 4);
    mFtab[0xA0] = Inst("LDY", &X::InstrLDY, &X::LoadIMM,  2);
    mFtab[0xA4] = Inst("LDY", &X::InstrLDY, &X::LoadZPG,  3);
    mFtab[0xB4] = Inst("LDY", &X::InstrLDY, &X::LoadZPGX, 4);
    mFtab[0xAC] = Inst("LDY", &X::InstrLDY, &X::LoadABS,  4);
    mFtab[0xBC] = Inst("LDY", &X::InstrLDY, &X::LoadABSX, 4);
    mFtab[0x4A] = Inst("LSR", &X::InstrLSR, &X::LoadACC,  2);
    mFtab[0x46] = Inst("LSR", &X::InstrLSR, &X::LoadZPG,  5);
    mFtab[0x56] = Inst("LSR", &X::InstrLSR, &X::LoadZPGX, 6);
    mFtab[0x4E] = Inst("LSR", &X::InstrLSR, &X::LoadABS,  6);
    mFtab[0x5E] = Inst("LSR", &X::InstrLSR, &X::LoadABSX, 7);

    mFtab[0xEA] = Inst("NOP",  &X::InstrNOP, &X::LoadIMP,  2);

    mFtab[0x09] = Inst("ORA", &X::InstrORA, &X::LoadIMM,  2);
    mFtab[0x05] = Inst("ORA", &X::InstrORA, &X::LoadZPG,  3);
    mFtab[0x15] = Inst("ORA", &X::InstrORA, &X::LoadZPGX, 4);
    mFtab[0x0D] = Inst("ORA", &X::InstrORA, &X::LoadABS,  4);
    mFtab[0x1D] = Inst("ORA", &X::InstrORA, &X::LoadABSX, 4);
    mFtab[0x19] = Inst("ORA", &X::InstrORA, &X::LoadABSY, 4);
    mFtab[0x01] = Inst("ORA", &X::InstrORA, &X::LoadINDX, 6);
    mFtab[0x11] = Inst("ORA", &X::InstrORA, &X::LoadINDY, 5);

    mFtab[0x68] = Inst("PLA", &X::InstrPLA, &X::LoadIMP, 4);
    mFtab[0x08] = Inst("PHP", &X::InstrPHP, &X::LoadIMP, 3);
    mFtab[0x48] = Inst("PHA", &X::InstrPHA, &X::LoadIMP, 3);
    mFtab[0x28] = Inst("PLP", &X::InstrPLP, &X::LoadIMP, 4);

    mFtab[0x2A] = Inst("ROL", &X::InstrROL, &X::LoadACC,  2);
    mFtab[0x26] = Inst("ROL", &X::InstrROL, &X::LoadZPG,  5);
    mFtab[0x36] = Inst("ROL", &X::InstrROL, &X::LoadZPGX, 6);
    mFtab[0x2E] = Inst("ROL", &X::InstrROL, &X::LoadABS,  6);
    mFtab[0x3E] = Inst("ROL", &X::InstrROL, &X::LoadABSX, 7);
    mFtab[0x6A] = Inst("ROR", &X::InstrROR, &X::LoadACC,  2);
    mFtab[0x66] = Inst("ROR", &X::InstrROR, &X::LoadZPG,  5);
    mFtab[0x76] = Inst("ROR", &X::InstrROR, &X::LoadZPGX, 6);
    mFtab[0x6E] = Inst("ROR", &X::InstrROR, &X::LoadABS,  6);
    mFtab[0x7E] = Inst("ROR", &X::InstrROR, &X::LoadABSX, 7);
    mFtab[0x40] = Inst("RTI", &X::InstrRTI, &X::LoadIMP,  6);
    mFtab[0x60] = Inst("RTS", &X::InstrRTS, &X::LoadIMP,  6);

    mFtab[0xE9] = Inst("SBC", &X::InstrSBC, &X::LoadIMM,  2);
    mFtab[0xE5] = Inst("SBC", &X::InstrSBC, &X::LoadZPG,  3);
    mFtab[0xF5] = Inst("SBC", &X::InstrSBC, &X::LoadZPGX, 4);
    mFtab[0xED] = Inst("SBC", &X::InstrSBC, &X::LoadABS,  4);
    mFtab[0xFD] = Inst("SBC", &X::InstrSBC, &X::LoadABSX, 4);
    mFtab[0xF9] = Inst("SBC", &X::InstrSBC, &X::LoadABSY, 4);
    mFtab[0xE1] = Inst("SBC", &X::InstrSBC, &X::LoadINDX, 6);
    mFtab[0xF1] = Inst("SBC", &X::InstrSBC, &X::LoadINDY, 5);
    mFtab[0x38] = Inst("SEC", &X::InstrSEC, &X::LoadIMP,  2);
    mFtab[0xF8] = Inst("SED", &X::InstrSED, &X::LoadIMP,  2);
    mFtab[0x78] = Inst("SEI", &X::InstrSEI, &X::LoadIMP,  2);
    mFtab[0x85] = Inst("STA", &X::InstrSTA, &X::LoadZPG,  3);
    mFtab[0x95] = Inst("STA", &X::InstrSTA, &X::LoadZPGX, 4);
    mFtab[0x8D] = Inst("STA", &X::InstrSTA, &X::LoadABS,  4);
    mFtab[0x9D] = Inst("STA", &X::InstrSTA, &X::LoadABSX, 5);
    mFtab[0x99] = Inst("STA", &X::InstrSTA, &X::LoadABSY, 5);
    mFtab[0x81] = Inst("STA", &X::InstrSTA, &X::LoadINDX, 6);
    mFtab[0x91] = Inst("STA", &X::InstrSTA, &X::LoadINDY, 6);
    mFtab[0x86] = Inst("STX", &X::InstrSTX, &X::LoadZPG,  3);
    mFtab[0x96] = Inst("STX", &X::InstrSTX, &X::LoadZPGY, 4);
    mFtab[0x8E] = Inst("STX", &X::InstrSTX, &X::LoadABS,  4);
    mFtab[0x84] = Inst("STY", &X::InstrSTY, &X::LoadZPG,  3);
    mFtab[0x94] = Inst("STY", &X::InstrSTY, &X::LoadZPGX, 4);
    mFtab[0x8C] = Inst("STY", &X::InstrSTY, &X::LoadABS,  4);

    mFtab[0xAA] = Inst("TAX", &X::InstrTAX, &X::LoadIMP, 2);
    mFtab[0xA8] = Inst("TAY", &X::InstrTAY, &X::LoadIMP, 2);
    mFtab[0xBA] = Inst("TSX", &X::InstrTSX, &X::LoadIMP, 2);
    mFtab[0x8A] = Inst("TXA", &X::InstrTXA, &X::LoadIMP, 2);
    mFtab[0x9A] = Inst("TXS", &X::InstrTXS, &X::LoadIMP, 2);
    mFtab[0x98] = Inst("TYA", &X::InstrTYA, &X::LoadIMP, 2);
}

