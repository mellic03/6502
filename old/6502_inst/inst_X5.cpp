
#include <stddef.h>
#include <stdint.h>

#include "../cpu_6502.hpp"


static void inst_05_ORA_zpg( cpu_6502 &cpu )
{

}


static void inst_15_ORA_zpg_x( cpu_6502 &cpu )
{

}


static void inst_25_AND_zpg( cpu_6502 &cpu )
{

}


static void inst_35_AND_zpg_x( cpu_6502 &cpu )
{

}


static void inst_45_EOR_zpg( cpu_6502 &cpu )
{

}


static void inst_55_EOR_zpg_x( cpu_6502 &cpu )
{

}


static void inst_65_ADC_zpg( cpu_6502 &cpu )
{

}


static void inst_75_ADC_zpg_x( cpu_6502 &cpu )
{

}


static void inst_85_STA_zpg( cpu_6502 &cpu )
{

}


static void inst_95_STA_zpg_x( cpu_6502 &cpu )
{

}


static void inst_A5_LDA_zpg( cpu_6502 &cpu )
{

}


static void inst_B5_LDA_zpg_x( cpu_6502 &cpu )
{

}


static void inst_C5_CMP_zpg( cpu_6502 &cpu )
{

}


static void inst_D5_CMP_zpg_x( cpu_6502 &cpu )
{

}


static void inst_E5_SBC_zpg( cpu_6502 &cpu )
{

}


static void inst_F5_SBC_zpg_x( cpu_6502 &cpu )
{

}



void LoadInst_6502_X5( ftable_6502 &table )
{
    auto &I = table.LOHI[0x05];
    I[0x00] = inst_05_ORA_zpg;
    I[0x01] = inst_15_ORA_zpg_x;
    I[0x02] = inst_25_AND_zpg;
    I[0x03] = inst_35_AND_zpg_x;
    I[0x04] = inst_45_EOR_zpg;
    I[0x05] = inst_55_EOR_zpg_x;
    I[0x06] = inst_65_ADC_zpg;
    I[0x07] = inst_75_ADC_zpg_x;
    I[0x08] = inst_85_STA_zpg;
    I[0x09] = inst_95_STA_zpg_x;
    I[0x0A] = inst_A5_LDA_zpg;
    I[0x0B] = inst_B5_LDA_zpg_x;
    I[0x0C] = inst_C5_CMP_zpg;
    I[0x0D] = inst_D5_CMP_zpg_x;
    I[0x0E] = inst_E5_SBC_zpg;
    I[0x0F] = inst_F5_SBC_zpg_x;
}
