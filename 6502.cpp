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
    ftab[0x20] = Inst(&cpu6502::LoadABS, &cpu6502::InstrJMP);
    ftab[0x20] = Inst(&cpu6502::LoadIND, &cpu6502::InstrJMP);

    ftab[0x01] = Inst(&cpu6502::LoadINDX, &cpu6502::InstrORA);
    ftab[0x11] = Inst(&cpu6502::LoadINDY, &cpu6502::InstrORA);
    ftab[0x21] = Inst(&cpu6502::LoadINDX, &cpu6502::InstrAND);
    ftab[0x31] = Inst(&cpu6502::LoadINDY, &cpu6502::InstrAND);

}

void cpu6502::LoadProgram( uint8_t *program, uint16_t size )
{
    memcpy(mMem, program, size);
}

void cpu6502::Run()
{
    // mRunning = true;

    while (!mInvalidOp)
    {
        mCurrOp = read08(PC++);
        ftab[mCurrOp].exec(*this);
    }

    printf("Invalid opcode (0x%02X)\n", mCurrOp);
}


