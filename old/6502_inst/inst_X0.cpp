
#include <stddef.h>
#include <stdint.h>

#include "../cpu_6502.hpp"



static void inst_00_BRK_impl( cpu_6502 &cpu )
{
    uint8_t *p = cpu.mInstPtr + 1;
    // [op] [pad] ...

    cpu.push16(cpu.PC_word + 2);

    cpu.SSR.B = 1;
    cpu.push08(cpu.SSR_byte);

}

static void inst_10_BPL_rel( cpu_6502 &cpu )
{

}

static void inst_20_JSR_abs( cpu_6502 &cpu )
{

}

static void inst_30_BMI_rel( cpu_6502 &cpu )
{

}

static void inst_40_RTI_impl( cpu_6502 &cpu )
{

}

static void inst_50_BVC_rel( cpu_6502 &cpu )
{

}

static void inst_60_RTS_impl( cpu_6502 &cpu )
{

}

static void inst_70_BVS_rel( cpu_6502 &cpu )
{

}

static void inst_80_invld_op( cpu_6502 &cpu )
{
    cpu.mRunning = false;
}

static void inst_90_BCC_rel( cpu_6502 &cpu )
{

}

static void inst_A0_LDY_imm( cpu_6502 &cpu )
{

}

static void inst_B0_BCS_rel( cpu_6502 &cpu )
{

}

static void inst_C0_CPY_imm( cpu_6502 &cpu )
{

}

static void inst_D0_BNE_rel( cpu_6502 &cpu )
{

}

static void inst_E0_CPX_imm( cpu_6502 &cpu )
{

}

static void inst_F0_BEQ_rel( cpu_6502 &cpu )
{

}


void LoadInst_6502_X0( ftable_6502 &table )
{
    auto &I = table.LOHI[0x00];
    I[0x00] = inst_00_BRK_impl;
    I[0x01] = inst_10_BPL_rel;
    I[0x02] = inst_20_JSR_abs;
    I[0x03] = inst_30_BMI_rel;
    I[0x04] = inst_40_RTI_impl;
    I[0x05] = inst_50_BVC_rel;
    I[0x06] = inst_60_RTS_impl;
    I[0x07] = inst_70_BVS_rel;
    I[0x08] = inst_80_invld_op;
    I[0x09] = inst_90_BCC_rel;
    I[0x0A] = inst_A0_LDY_imm;
    I[0x0B] = inst_B0_BCS_rel;
    I[0x0C] = inst_C0_CPY_imm;
    I[0x0D] = inst_D0_BNE_rel;
    I[0x0E] = inst_E0_CPX_imm;
    I[0x0F] = inst_F0_BEQ_rel;
}
