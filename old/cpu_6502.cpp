#include "cpu_6502.hpp"
#include <stdio.h>


static void inst_XX_unimpl( cpu_6502 &cpu )
{
    uint8_t lo = cpu.mInst.lo;
    uint8_t hi = cpu.mInst.hi;
    printf("Instruction not implemented (0x%02u%02u).\n", hi, lo);
    cpu.mRunning = false;
}



extern void LoadInst_6502_X0( ftable_6502& );
extern void LoadInst_6502_X1( ftable_6502& );
extern void LoadInst_6502_X5( ftable_6502& );
extern void LoadInst_6502_X6( ftable_6502& );
extern void LoadInst_6502_X7( ftable_6502& );
extern void LoadInst_6502_X8( ftable_6502& );

ftable_6502::ftable_6502()
{
    for (int i=0; i<16; i++)
        for (int j=0; j<16; j++)
            LOHI[i][j] = inst_XX_unimpl;

    LoadInst_6502_X0(*this);
    LoadInst_6502_X1(*this);
    LoadInst_6502_X5(*this);
    LoadInst_6502_X6(*this);
    LoadInst_6502_X7(*this);
    LoadInst_6502_X8(*this);
}


cpu_6502::cpu_6502( uint8_t *program )
:   cpu_6502_registers(program),
    mInstPtr(program),
    mProg(program),
    mRunning(true)
{
    
}


uint8_t*
cpu_6502::fetch()
{
    mInstPtr = mProg + PC_word;
    return mInstPtr;
}


cpu_6502_inst
cpu_6502::decode( const uint8_t *ptr )
{
    cpu_6502_inst inst = {*ptr};
    return inst;
}


bool
cpu_6502::execute( const cpu_6502_inst &inst )
{
    mInst = inst;
    (mFTab.get(inst.lo, inst.hi))(*this);
    PC_word += 1;
    return true;
}

