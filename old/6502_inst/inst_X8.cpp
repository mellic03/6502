
#include <stddef.h>
#include <stdint.h>

#include "../cpu_6502.hpp"



static void inst_08_PHP_impl( cpu_6502 &cpu )
{

}

static void inst_18_CLC_impl( cpu_6502 &cpu )
{

}

static void inst_28_PLP_impl( cpu_6502 &cpu )
{

}

static void inst_38_SEC_impl( cpu_6502 &cpu )
{

}

static void inst_48_PHA_impl( cpu_6502 &cpu )
{

}

static void inst_58_CLI_impl( cpu_6502 &cpu )
{

}

static void inst_68_PLA_impl( cpu_6502 &cpu )
{

}

static void inst_78_SEI_impl( cpu_6502 &cpu )
{

}

static void inst_88_DEY_impl( cpu_6502 &cpu )
{

}

static void inst_98_TYA_impl( cpu_6502 &cpu )
{

}

static void inst_A8_TAY_impl( cpu_6502 &cpu )
{

}

static void inst_B8_CLV_impl( cpu_6502 &cpu )
{

}

static void inst_C8_INY_impl( cpu_6502 &cpu )
{

}

static void inst_D8_CLD_impl( cpu_6502 &cpu )
{

}

static void inst_E8_INX_impl( cpu_6502 &cpu )
{

}

static void inst_F8_SED_impl( cpu_6502 &cpu )
{

}



void LoadInst_6502_X8( ftable_6502 &table )
{
    auto &I = table.LOHI[0x08];
    I[0x00] = inst_08_PHP_impl;
    I[0x01] = inst_18_CLC_impl;
    I[0x02] = inst_28_PLP_impl;
    I[0x03] = inst_38_SEC_impl;
    I[0x04] = inst_48_PHA_impl;
    I[0x05] = inst_58_CLI_impl;
    I[0x06] = inst_68_PLA_impl;
    I[0x07] = inst_78_SEI_impl;
    I[0x08] = inst_88_DEY_impl;
    I[0x09] = inst_98_TYA_impl;
    I[0x0A] = inst_A8_TAY_impl;
    I[0x0B] = inst_B8_CLV_impl;
    I[0x0C] = inst_C8_INY_impl;
    I[0x0D] = inst_D8_CLD_impl;
    I[0x0E] = inst_E8_INX_impl;
    I[0x0F] = inst_F8_SED_impl;
}
