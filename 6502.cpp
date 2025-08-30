#include "6502.hpp"
#include <stdio.h>
#include <string.h>


// using AddrFn = uint16_t (*)(cpu6502&, uint16_t);
// using ExecFn = void (*)(cpu6502&, uint16_t);

// template <AddrFn fA, ExecFn fE>
// struct Inst: public cpu6502::InstBase
// {
//     virtual void exec( cpu6502 &cpu ) final
//     {
//         (cpu6502::*fA)();
//         fE(cpu, fA(cpu, cpu.PC++));
//     }
// };


cpu6502::cpu6502()
:   mInvalidOp(0), mCurrOp(0),
    AC(0x00), XR(0x00), YR(0x00), SP(0x00), PC(0x0000)
{
    using X = cpu6502;

    for (int i=0; i<256; i++)
    {
        ftab[i] = Inst(&X::LoadACC, &X::InstrUnimp);
    }


    ftab[0x18] = Inst(&X::LoadIMP, &X::InstrCLC);
    ftab[0xD8] = Inst(&X::LoadIMP, &X::InstrCLD);
    ftab[0x58] = Inst(&X::LoadIMP, &X::InstrCLI);
    ftab[0xB8] = Inst(&X::LoadIMP, &X::InstrCLV);


    ftab[0xC9] = Inst(&X::LoadIMM,  &X::InstrCMP);
    ftab[0xC5] = Inst(&X::LoadZPG,  &X::InstrCMP);
    ftab[0xD5] = Inst(&X::LoadZPGX, &X::InstrCMP);
    ftab[0xCD] = Inst(&X::LoadABS,  &X::InstrCMP);
    ftab[0xDD] = Inst(&X::LoadABSX, &X::InstrCMP);
    ftab[0xD9] = Inst(&X::LoadABSY, &X::InstrCMP);
    ftab[0xC1] = Inst(&X::LoadINDX, &X::InstrCMP);
    ftab[0xD1] = Inst(&X::LoadINDY, &X::InstrCMP);

    ftab[0xE0] = Inst(&X::LoadIMM, &X::InstrCPX);
    ftab[0xE4] = Inst(&X::LoadZPG, &X::InstrCPX);
    ftab[0xEC] = Inst(&X::LoadABS, &X::InstrCPX);

    ftab[0xC0] = Inst(&X::LoadIMM, &X::InstrCPY);
    ftab[0xC4] = Inst(&X::LoadZPG, &X::InstrCPY);
    ftab[0xCC] = Inst(&X::LoadABS, &X::InstrCPY);


    ftab[0xB0] = Inst(&X::LoadREL, &X::InstrBCS);
    ftab[0xF0] = Inst(&X::LoadREL, &X::InstrBEQ);
    ftab[0x30] = Inst(&X::LoadREL, &X::InstrBMI);
    ftab[0xD0] = Inst(&X::LoadREL, &X::InstrBNE);
    ftab[0x10] = Inst(&X::LoadREL, &X::InstrBPL);
    ftab[0x00] = Inst(&X::LoadIMP, &X::InstrBRK);
    ftab[0x50] = Inst(&X::LoadREL, &X::InstrBVC);
    ftab[0x70] = Inst(&X::LoadREL, &X::InstrBVS);


    ftab[0x4C] = Inst(&X::LoadABS, &X::InstrJMP);
    ftab[0x6C] = Inst(&X::LoadIND, &X::InstrJMP);
    ftab[0x6C] = Inst(&X::LoadABS, &X::InstrJSR);


    ftab[0xA9] = Inst(&X::LoadIMM,  &X::InstrLDA);
    ftab[0xA5] = Inst(&X::LoadZPG,  &X::InstrLDA);
    ftab[0xB5] = Inst(&X::LoadZPGX, &X::InstrLDA);
    ftab[0xAD] = Inst(&X::LoadABS,  &X::InstrLDA);
    ftab[0xBD] = Inst(&X::LoadABSX, &X::InstrLDA);
    ftab[0xB9] = Inst(&X::LoadABSY, &X::InstrLDA);
    ftab[0xA1] = Inst(&X::LoadINDX, &X::InstrLDA);
    ftab[0xB1] = Inst(&X::LoadINDY, &X::InstrLDA);

    ftab[0xA2] = Inst(&X::LoadIMM,  &X::InstrLDX);
    ftab[0xA6] = Inst(&X::LoadZPG,  &X::InstrLDX);
    ftab[0xB6] = Inst(&X::LoadZPGY, &X::InstrLDX);
    ftab[0xAE] = Inst(&X::LoadABS,  &X::InstrLDX);
    ftab[0xBE] = Inst(&X::LoadABSY, &X::InstrLDX);

    ftab[0xA0] = Inst(&X::LoadIMM,  &X::InstrLDY);
    ftab[0xA4] = Inst(&X::LoadZPG,  &X::InstrLDY);
    ftab[0xB4] = Inst(&X::LoadZPGX, &X::InstrLDY);
    ftab[0xAC] = Inst(&X::LoadABS,  &X::InstrLDY);
    ftab[0xBC] = Inst(&X::LoadABSX, &X::InstrLDY);


    ftab[0x01] = Inst(&X::LoadINDX, &X::InstrORA);
    ftab[0x11] = Inst(&X::LoadINDY, &X::InstrORA);
    ftab[0x21] = Inst(&X::LoadINDX, &X::InstrAND);
    ftab[0x31] = Inst(&X::LoadINDY, &X::InstrAND);


    ftab[0x68] = Inst(&X::LoadIMP, &X::InstrPLA);
    ftab[0x08] = Inst(&X::LoadIMP, &X::InstrPHP);
    ftab[0x48] = Inst(&X::LoadIMP, &X::InstrPHA);
    ftab[0x28] = Inst(&X::LoadIMP, &X::InstrPLP);

}

void cpu6502::LoadProgram( uint8_t *program, uint16_t offset, uint16_t size )
{
    if ((size_t)offset + (size_t)size < 65535)
    {
        memcpy(mMem+offset, program, size);
    }
}

void cpu6502::WriteResetVector( uint16_t addr )
{
    write16(0xFFFC, addr);
}

void cpu6502::Run()
{
    PC = read16(0xFFFC); // mMem[0xFFFC];

    printf("PC=0x%02x\n", PC);

    while (!mInvalidOp)
    {
        mCurrOp = read08(PC++);
        printf("opcode=0x%02x\n", mCurrOp);
        ftab[mCurrOp].exec(*this);
    }

    printf("Invalid opcode (0x%02X)\n", mCurrOp);
}


