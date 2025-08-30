
#include <stddef.h>
#include <stdint.h>

#include "../cpu_6502.hpp"


static void inst_01_ORA_X_ind( cpu_6502 &cpu )
{
    uint8_t *p = cpu.mInstPtr;
    uint16_t X = (p[0] << 8) + p[1];

    cpu.AC += cpu.mRAM[ ]
}

static void inst_11_ORA_ind_Y( cpu_6502 &cpu )
{

}

static void inst_21_AND_X_ind( cpu_6502 &cpu )
{

}

static void inst_31_AND_ind_Y( cpu_6502 &cpu )
{

}

static void inst_41_EOR_X_ind( cpu_6502 &cpu )
{

}

static void inst_51_EOR_ind_Y( cpu_6502 &cpu )
{

}

static void inst_61_ADC_X_ind( cpu_6502 &cpu )
{

}

static void inst_71_ADC_ind_Y( cpu_6502 &cpu )
{

}

static void inst_81_STA_X_ind( cpu_6502 &cpu )
{

}

static void inst_91_STA_ind_Y( cpu_6502 &cpu )
{

}

static void inst_A1_LDA_X_ind( cpu_6502 &cpu )
{

}

static void inst_B1_LDA_ind_Y( cpu_6502 &cpu )
{

}

static void inst_C1_CMP_X_ind( cpu_6502 &cpu )
{

}

static void inst_D1_CMP_ind_Y( cpu_6502 &cpu )
{

}

static void inst_E1_SBC_X_ind( cpu_6502 &cpu )
{

}

static void inst_F1_SBC_ind_Y( cpu_6502 &cpu )
{

}


void LoadInst_6502_X1( ftable_6502 &table )
{
    auto &I = table.LOHI[0x01];
    I[0x00] = inst_01_ORA_X_ind;
    I[0x01] = inst_11_ORA_ind_Y;
    I[0x02] = inst_21_AND_X_ind;
    I[0x03] = inst_31_AND_ind_Y;
    I[0x04] = inst_41_EOR_X_ind;
    I[0x05] = inst_51_EOR_ind_Y;
    I[0x06] = inst_61_ADC_X_ind;
    I[0x07] = inst_71_ADC_ind_Y;
    I[0x08] = inst_81_STA_X_ind;
    I[0x09] = inst_91_STA_ind_Y;
    I[0x0A] = inst_A1_LDA_X_ind;
    I[0x0B] = inst_B1_LDA_ind_Y;
    I[0x0C] = inst_C1_CMP_X_ind;
    I[0x0D] = inst_D1_CMP_ind_Y;
    I[0x0E] = inst_E1_SBC_X_ind;
    I[0x0F] = inst_F1_SBC_ind_Y;
}
