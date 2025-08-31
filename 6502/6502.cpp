#include "6502.hpp"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>


void cpu6502::LoadROM( uint8_t *rom )
{
    for (size_t i=0; i<65535; i++)
        wtbus(i, rom[i]);
    PC = rdbus16(0xFFFC);
    printf("PC:  0x%02x\n\n", PC);
}


void cpu6502::Tick()
{
    if (mInvalidOp)
    {
        return;
    }

    // printf("[6502]\n");

    mCurrOp = _adv08();
    mFtab[mCurrOp](*this);

    if (mInvalidOp)
    {
        printf("Invalid opcode (0x%02X)\n", mCurrOp);
        return;
    }

    printf("op:  0x%02X\n", mCurrOp);
    printf("AC:  0x%02X\n", AC);
    printf("SSR: ");
    for (int i=7; i>=0; i--)
        printf((SSR_byte & (1<<i)) ? "1" : "0");
    printf("\n");
    printf("   : NVB DIZC\n");
    printf("\n");
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
    SP(0xFF), PC(0x0000)
{
    using X = cpu6502;

    for (int i=0; i<256; i++)
    {
        mFtab[i] = Inst(&X::InstrUnimp, &X::LoadACC, 1);
    }

    mFtab[0x69] = Inst(&X::InstrADC, &X::LoadIMM,  2);
    mFtab[0x65] = Inst(&X::InstrADC, &X::LoadZPG,  3);
    mFtab[0x75] = Inst(&X::InstrADC, &X::LoadZPGX, 4);
    mFtab[0x6D] = Inst(&X::InstrADC, &X::LoadABS,  4);
    mFtab[0x7D] = Inst(&X::InstrADC, &X::LoadABSX, 4);
    mFtab[0x79] = Inst(&X::InstrADC, &X::LoadABSY, 4);
    mFtab[0x61] = Inst(&X::InstrADC, &X::LoadINDX, 6);
    mFtab[0x71] = Inst(&X::InstrADC, &X::LoadINDY, 5);


    mFtab[0x29] = Inst(&X::InstrAND, &X::LoadIMM,  2);
    mFtab[0x25] = Inst(&X::InstrAND, &X::LoadZPG,  3);
    mFtab[0x35] = Inst(&X::InstrAND, &X::LoadZPGX, 4);
    mFtab[0x2D] = Inst(&X::InstrAND, &X::LoadABS,  4);
    mFtab[0x3D] = Inst(&X::InstrAND, &X::LoadABSX, 4);
    mFtab[0x39] = Inst(&X::InstrAND, &X::LoadABSY, 4);
    mFtab[0x21] = Inst(&X::InstrAND, &X::LoadINDX, 6);
    mFtab[0x31] = Inst(&X::InstrAND, &X::LoadINDY, 5);


    mFtab[0xB0] = Inst(&X::InstrBCS, &X::LoadREL, 2);
    mFtab[0xF0] = Inst(&X::InstrBEQ, &X::LoadREL, 2);
    mFtab[0x30] = Inst(&X::InstrBMI, &X::LoadREL, 2);
    mFtab[0xD0] = Inst(&X::InstrBNE, &X::LoadREL, 2);
    mFtab[0x10] = Inst(&X::InstrBPL, &X::LoadREL, 2);
    mFtab[0x00] = Inst(&X::InstrBRK, &X::LoadIMP, 7);
    mFtab[0x50] = Inst(&X::InstrBVC, &X::LoadREL, 2);
    mFtab[0x70] = Inst(&X::InstrBVS, &X::LoadREL, 2);


    mFtab[0x18] = Inst(&X::InstrCLC, &X::LoadIMP, 2);
    mFtab[0xD8] = Inst(&X::InstrCLD, &X::LoadIMP, 2);
    mFtab[0x58] = Inst(&X::InstrCLI, &X::LoadIMP, 2);
    mFtab[0xB8] = Inst(&X::InstrCLV, &X::LoadIMP, 2);


    mFtab[0xC9] = Inst(&X::InstrCMP, &X::LoadIMM,  2);
    mFtab[0xC5] = Inst(&X::InstrCMP, &X::LoadZPG,  3);
    mFtab[0xD5] = Inst(&X::InstrCMP, &X::LoadZPGX, 4);
    mFtab[0xCD] = Inst(&X::InstrCMP, &X::LoadABS,  4);
    mFtab[0xDD] = Inst(&X::InstrCMP, &X::LoadABSX, 4);
    mFtab[0xD9] = Inst(&X::InstrCMP, &X::LoadABSY, 4);
    mFtab[0xC1] = Inst(&X::InstrCMP, &X::LoadINDX, 6);
    mFtab[0xD1] = Inst(&X::InstrCMP, &X::LoadINDY, 5);


    mFtab[0xE0] = Inst(&X::InstrCPX, &X::LoadIMM, 2);
    mFtab[0xE4] = Inst(&X::InstrCPX, &X::LoadZPG, 3);
    mFtab[0xEC] = Inst(&X::InstrCPX, &X::LoadABS, 4);
    mFtab[0xC0] = Inst(&X::InstrCPY, &X::LoadIMM, 2);
    mFtab[0xC4] = Inst(&X::InstrCPY, &X::LoadZPG, 3);
    mFtab[0xCC] = Inst(&X::InstrCPY, &X::LoadABS, 4);


    mFtab[0x4C] = Inst(&X::InstrJMP, &X::LoadABS, 3);
    mFtab[0x6C] = Inst(&X::InstrJMP, &X::LoadIND, 5);
    mFtab[0x6C] = Inst(&X::InstrJSR, &X::LoadABS, 6);


    mFtab[0xA9] = Inst(&X::InstrLDA, &X::LoadIMM,  2);
    mFtab[0xA5] = Inst(&X::InstrLDA, &X::LoadZPG,  3);
    mFtab[0xB5] = Inst(&X::InstrLDA, &X::LoadZPGX, 4);
    mFtab[0xAD] = Inst(&X::InstrLDA, &X::LoadABS,  4);
    mFtab[0xBD] = Inst(&X::InstrLDA, &X::LoadABSX, 4);
    mFtab[0xB9] = Inst(&X::InstrLDA, &X::LoadABSY, 4);
    mFtab[0xA1] = Inst(&X::InstrLDA, &X::LoadINDX, 6);
    mFtab[0xB1] = Inst(&X::InstrLDA, &X::LoadINDY, 5);
    mFtab[0xA2] = Inst(&X::InstrLDX, &X::LoadIMM,  2);
    mFtab[0xA6] = Inst(&X::InstrLDX, &X::LoadZPG,  3);
    mFtab[0xB6] = Inst(&X::InstrLDX, &X::LoadZPGY, 4);
    mFtab[0xAE] = Inst(&X::InstrLDX, &X::LoadABS,  4);
    mFtab[0xBE] = Inst(&X::InstrLDX, &X::LoadABSY, 4);
    mFtab[0xA0] = Inst(&X::InstrLDY, &X::LoadIMM,  2);
    mFtab[0xA4] = Inst(&X::InstrLDY, &X::LoadZPG,  3);
    mFtab[0xB4] = Inst(&X::InstrLDY, &X::LoadZPGX, 4);
    mFtab[0xAC] = Inst(&X::InstrLDY, &X::LoadABS,  4);
    mFtab[0xBC] = Inst(&X::InstrLDY, &X::LoadABSX, 4);


    mFtab[0x09] = Inst(&X::InstrORA, &X::LoadIMM,  2);
    mFtab[0x05] = Inst(&X::InstrORA, &X::LoadZPG,  3);
    mFtab[0x15] = Inst(&X::InstrORA, &X::LoadZPGX, 4);
    mFtab[0x0D] = Inst(&X::InstrORA, &X::LoadABS,  4);
    mFtab[0x1D] = Inst(&X::InstrORA, &X::LoadABSX, 4);
    mFtab[0x19] = Inst(&X::InstrORA, &X::LoadABSY, 4);
    mFtab[0x01] = Inst(&X::InstrORA, &X::LoadINDX, 6);
    mFtab[0x11] = Inst(&X::InstrORA, &X::LoadINDY, 5);


    mFtab[0x68] = Inst(&X::InstrPLA, &X::LoadIMP, 4);
    mFtab[0x08] = Inst(&X::InstrPHP, &X::LoadIMP, 3);
    mFtab[0x48] = Inst(&X::InstrPHA, &X::LoadIMP, 3);
    mFtab[0x28] = Inst(&X::InstrPLP, &X::LoadIMP, 4);


    mFtab[0xE9] = Inst(&X::InstrSBC, &X::LoadIMM,  2);
    mFtab[0xE5] = Inst(&X::InstrSBC, &X::LoadZPG,  3);
    mFtab[0xF5] = Inst(&X::InstrSBC, &X::LoadZPGX, 4);
    mFtab[0xED] = Inst(&X::InstrSBC, &X::LoadABS,  4);
    mFtab[0xFD] = Inst(&X::InstrSBC, &X::LoadABSX, 4);
    mFtab[0xF9] = Inst(&X::InstrSBC, &X::LoadABSY, 4);
    mFtab[0xE1] = Inst(&X::InstrSBC, &X::LoadINDX, 6);
    mFtab[0xF1] = Inst(&X::InstrSBC, &X::LoadINDY, 5);


    mFtab[0x85] = Inst(&X::InstrSTA, &X::LoadZPG,  3);
    mFtab[0x95] = Inst(&X::InstrSTA, &X::LoadZPGX, 4);
    mFtab[0x8D] = Inst(&X::InstrSTA, &X::LoadABS,  4);
    mFtab[0x9D] = Inst(&X::InstrSTA, &X::LoadABSX, 5);
    mFtab[0x99] = Inst(&X::InstrSTA, &X::LoadABSY, 5);
    mFtab[0x81] = Inst(&X::InstrSTA, &X::LoadINDX, 6);
    mFtab[0x91] = Inst(&X::InstrSTA, &X::LoadINDY, 6);

}

