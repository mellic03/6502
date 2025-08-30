
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../cpu_6502.hpp"


static void inst_06_ASL_zpg( cpu_6502 &cpu )
{
    printf("WOOOO\n");
}


static void inst_16_ASL_zpg_x( cpu_6502 &cpu )
{

}


static void inst_26_ROL_zpg( cpu_6502 &cpu )
{

}


static void inst_36_ROL_zpg_x( cpu_6502 &cpu )
{

}


static void inst_46_LSR_zpg( cpu_6502 &cpu )
{

}


static void inst_56_LSR_zpg_x( cpu_6502 &cpu )
{

}


static void inst_66_ROR_zpg( cpu_6502 &cpu )
{

}


static void inst_76_ROR_zpg_x( cpu_6502 &cpu )
{

}


static void inst_86_STX_zpg( cpu_6502 &cpu )
{

}


static void inst_96_STX_zpg_y( cpu_6502 &cpu )
{

}


static void inst_A6_LDX_zpg( cpu_6502 &cpu )
{

}


static void inst_B6_LDX_zpg_y( cpu_6502 &cpu )
{

}


static void inst_C6_DEC_zpg( cpu_6502 &cpu )
{

}


static void inst_D6_DEC_zpg_x( cpu_6502 &cpu )
{

}


static void inst_E6_INC_zpg( cpu_6502 &cpu )
{

}


static void inst_F6_INC_zpg_x( cpu_6502 &cpu )
{

}



void LoadInst_6502_X6( ftable_6502 &table )
{
    auto &I = table.LOHI[0x06];
    I[0x00] = inst_06_ASL_zpg;
    I[0x01] = inst_16_ASL_zpg_x;
    I[0x02] = inst_26_ROL_zpg;
    I[0x03] = inst_36_ROL_zpg_x;
    I[0x04] = inst_46_LSR_zpg;
    I[0x05] = inst_56_LSR_zpg_x;
    I[0x06] = inst_66_ROR_zpg;
    I[0x07] = inst_76_ROR_zpg_x;
    I[0x08] = inst_86_STX_zpg;
    I[0x09] = inst_96_STX_zpg_y;
    I[0x0A] = inst_A6_LDX_zpg;
    I[0x0B] = inst_B6_LDX_zpg_y;
    I[0x0C] = inst_C6_DEC_zpg;
    I[0x0D] = inst_D6_DEC_zpg_x;
    I[0x0E] = inst_E6_INC_zpg;
    I[0x0F] = inst_F6_INC_zpg_x;
}
