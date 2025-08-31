#include "6502.hpp"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>


void cpu6502::LoadROM( uint8_t *rom )
{
    for (size_t i=0; i<65535; i++)
        wtbus(i, rom[i]);
    PC = rdbus16(0xFFFC);
    // printf("PC:  0x%02x\n\n", PC);
}


void cpu6502::Tick()
{
    if (mInvalidOp)
    {
        return;
    }

    // printf("[6502]\n");

    uint16_t mCurrPC = PC;
    mCurrOp = _adv08();
    printf("%04X  %02X %02X %02X  ", mCurrPC, mCurrOp, mBus[PC+0], mBus[PC+1]);
    // printf("%s\n", mFtab[mCurrOp].label);

    // A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 30 CYC:10

    printf(
        "\tA:%02X  X:%02X  Y:%02X  P:%02X  SP:%02X  PPU:   0,30 CYC:%lu\n",
        AC, XR, YR, SSR_byte, SP, mCycles
    );

    // printf("0x%04X  %s 0x%04X\n", mCurrPC, mFtab[mCurrOp].label, *(uint16_t*)(&mBus[PC]));
    mFtab[mCurrOp](*this);
    mCycles += mFtab[mCurrOp].nCycles;


    if (mInvalidOp)
    {
        printf("Invalid opcode (0x%02X)\n", mCurrOp);
        return;
    }

    // printf("op:  0x%02X\n", mCurrOp);
    // printf("AC:  0x%02X\n", AC);
    // printf("SSR: ");
    // for (int i=7; i>=0; i--)
    //     printf((SSR_byte & (1<<i)) ? "1" : "0");
    // printf("\n");
    // printf("   : NVB DIZC\n");
    // printf("\n");
}



void cpu6502::push08( uint8_t byte )
{
    wtbus(PAGE_STACK+SP, byte);
    SP -= 1;
}

void cpu6502::push16( uint16_t word )
{
    wtbus16(PAGE_STACK+SP, word);
    SP -= 2;
}

uint8_t cpu6502::pop08()
{
    SP += 1;
    return rdbus(PAGE_STACK+SP);
}

uint16_t cpu6502::pop16()
{
    SP += 2;
    return rdbus16(PAGE_STACK+SP);
}






cpu6502::cpu6502( iDataBus *bus )
:   BusAttachment(bus),
    SignalListener(1),
    mInvalidOp(0), mCurrOp(0),
    AC(0x00), XR(0x00), YR(0x00),
    SP(0xFD), PC(0x0000)
{
    using X = cpu6502;

    for (int i=0; i<256; i++)
    {
        mFtab[i] = Inst("Unimp",   &X::InstrUnimp, &X::LoadACC, 1);
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


    mFtab[0x18] = Inst("CLC",  &X::InstrCLC, &X::LoadIMP, 2);
    mFtab[0xD8] = Inst("CLD",  &X::InstrCLD, &X::LoadIMP, 2);
    mFtab[0x58] = Inst("CLI",  &X::InstrCLI, &X::LoadIMP, 2);
    mFtab[0xB8] = Inst("CLV",  &X::InstrCLV, &X::LoadIMP, 2);


    mFtab[0xC9] = Inst("CMP #oper   ", &X::InstrCMP, &X::LoadIMM,  2);
    mFtab[0xC5] = Inst("CMP oper    ", &X::InstrCMP, &X::LoadZPG,  3);
    mFtab[0xD5] = Inst("CMP oper,X  ", &X::InstrCMP, &X::LoadZPGX, 4);
    mFtab[0xCD] = Inst("CMP oper    ", &X::InstrCMP, &X::LoadABS,  4);
    mFtab[0xDD] = Inst("CMP oper,X  ", &X::InstrCMP, &X::LoadABSX, 4);
    mFtab[0xD9] = Inst("CMP oper,Y  ", &X::InstrCMP, &X::LoadABSY, 4);
    mFtab[0xC1] = Inst("CMP (oper,X)", &X::InstrCMP, &X::LoadINDX, 6);
    mFtab[0xD1] = Inst("CMP (oper),Y", &X::InstrCMP, &X::LoadINDY, 5);


    mFtab[0xE0] = Inst("CPX",  &X::InstrCPX, &X::LoadIMM, 2);
    mFtab[0xE4] = Inst("CPX",  &X::InstrCPX, &X::LoadZPG, 3);
    mFtab[0xEC] = Inst("CPX",  &X::InstrCPX, &X::LoadABS, 4);
    mFtab[0xC0] = Inst("CPY",  &X::InstrCPY, &X::LoadIMM, 2);
    mFtab[0xC4] = Inst("CPY",  &X::InstrCPY, &X::LoadZPG, 3);
    mFtab[0xCC] = Inst("CPY",  &X::InstrCPY, &X::LoadABS, 4);


    mFtab[0x49] = Inst("EOR #oper   ", &X::InstrEOR, &X::LoadIMM,  2);
    mFtab[0x45] = Inst("EOR oper    ", &X::InstrEOR, &X::LoadZPG,  3);
    mFtab[0x55] = Inst("EOR oper,X  ", &X::InstrEOR, &X::LoadZPGX, 4);
    mFtab[0x4D] = Inst("EOR oper    ", &X::InstrEOR, &X::LoadABS,  4);
    mFtab[0x5D] = Inst("EOR oper,X  ", &X::InstrEOR, &X::LoadABSX, 4);
    mFtab[0x59] = Inst("EOR oper,Y  ", &X::InstrEOR, &X::LoadABSY, 4);
    mFtab[0x41] = Inst("EOR (oper,X)", &X::InstrEOR, &X::LoadINDX, 6);
    mFtab[0x51] = Inst("EOR (oper),Y", &X::InstrEOR, &X::LoadINDY, 5);


    mFtab[0x4C] = Inst("JMP",  &X::InstrJMP, &X::LoadABS, 3);
    mFtab[0x6C] = Inst("JMP",  &X::InstrJMP, &X::LoadIND, 5);
    mFtab[0x20] = Inst("JSR",  &X::InstrJSR, &X::LoadABS, 6);


    mFtab[0xA9] = Inst("LDA #oper   ", &X::InstrLDA, &X::LoadIMM,  2);
    mFtab[0xA5] = Inst("LDA oper    ", &X::InstrLDA, &X::LoadZPG,  3);
    mFtab[0xB5] = Inst("LDA oper,X  ", &X::InstrLDA, &X::LoadZPGX, 4);
    mFtab[0xAD] = Inst("LDA oper    ", &X::InstrLDA, &X::LoadABS,  4);
    mFtab[0xBD] = Inst("LDA oper,X  ", &X::InstrLDA, &X::LoadABSX, 4);
    mFtab[0xB9] = Inst("LDA oper,Y  ", &X::InstrLDA, &X::LoadABSY, 4);
    mFtab[0xA1] = Inst("LDA (oper,X)", &X::InstrLDA, &X::LoadINDX, 6);
    mFtab[0xB1] = Inst("LDA (oper),Y", &X::InstrLDA, &X::LoadINDY, 5);


    mFtab[0xA2] = Inst("LDX #oper ", &X::InstrLDX, &X::LoadIMM,  2);
    mFtab[0xA6] = Inst("LDX oper  ", &X::InstrLDX, &X::LoadZPG,  3);
    mFtab[0xB6] = Inst("LDX oper,Y", &X::InstrLDX, &X::LoadZPGY, 4);
    mFtab[0xAE] = Inst("LDX oper  ", &X::InstrLDX, &X::LoadABS,  4);
    mFtab[0xBE] = Inst("LDX oper,Y", &X::InstrLDX, &X::LoadABSY, 4);
    mFtab[0xA0] = Inst("LDY #oper ", &X::InstrLDY, &X::LoadIMM,  2);
    mFtab[0xA4] = Inst("LDY oper  ", &X::InstrLDY, &X::LoadZPG,  3);
    mFtab[0xB4] = Inst("LDY oper,X", &X::InstrLDY, &X::LoadZPGX, 4);
    mFtab[0xAC] = Inst("LDY oper  ", &X::InstrLDY, &X::LoadABS,  4);
    mFtab[0xBC] = Inst("LDY oper,X", &X::InstrLDY, &X::LoadABSX, 4);

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

    mFtab[0x40] = Inst("RTI", &X::InstrRTI, &X::LoadIMP, 6);
    mFtab[0x60] = Inst("RTS", &X::InstrRTS, &X::LoadIMP, 6);

    mFtab[0xE9] = Inst("SBC", &X::InstrSBC, &X::LoadIMM,  2);
    mFtab[0xE5] = Inst("SBC", &X::InstrSBC, &X::LoadZPG,  3);
    mFtab[0xF5] = Inst("SBC", &X::InstrSBC, &X::LoadZPGX, 4);
    mFtab[0xED] = Inst("SBC", &X::InstrSBC, &X::LoadABS,  4);
    mFtab[0xFD] = Inst("SBC", &X::InstrSBC, &X::LoadABSX, 4);
    mFtab[0xF9] = Inst("SBC", &X::InstrSBC, &X::LoadABSY, 4);
    mFtab[0xE1] = Inst("SBC", &X::InstrSBC, &X::LoadINDX, 6);
    mFtab[0xF1] = Inst("SBC", &X::InstrSBC, &X::LoadINDY, 5);

    mFtab[0x38] = Inst("SEC", &X::InstrSEC, &X::LoadIMP,  2);

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
}

