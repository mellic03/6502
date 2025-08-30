#pragma once

#include "6502.hpp"


// template <typename T>
// struct Executor
// {   void operator()( cpu6502 &cpu, uint16_t src )
//     {
//         (T())(cpu, src);
//     }
// };

// struct ExAND: Executor<ExAND>
// {   void operator()( cpu6502 &cpu, uint16_t src )
//     {
//         cpu.AC &= cpu.Read(src);
//         cpu.SSR.N = (cpu.AC) ? 1 : 0;
//         cpu.SSR.Z = (!cpu.AC) ? 1 : 0;
//     }
// };

// struct ExORA: Executor<ExAND>
// {   void operator()( cpu6502 &cpu, uint16_t src )
//     {
//         cpu.AC |= cpu.Read(src);
//         cpu.SSR.N = (cpu.AC) ? 1 : 0;
//         cpu.SSR.Z = (!cpu.AC) ? 1 : 0;
//     }
// };


void Exec_ADC( cpu6502 &cpu, uint16_t src )
{

}

void Exec_AND( cpu6502 &cpu, uint16_t src )
{
    cpu.AC &= cpu.Read(src);
    cpu.SSR.N = (cpu.AC) ? 1 : 0;
    cpu.SSR.Z = (!cpu.AC) ? 1 : 0;
}

void Exec_CMP( cpu6502 &cpu, uint16_t src )
{

}

void Exec_EOR( cpu6502 &cpu, uint16_t src )
{

}

void Exec_LDA( cpu6502 &cpu, uint16_t src )
{

}

void Exec_ORA( cpu6502 &cpu, uint16_t src )
{
    cpu.AC |= cpu.Read(src);
    cpu.SSR.N = (cpu.AC) ? 1 : 0;
    cpu.SSR.Z = (!cpu.AC) ? 1 : 0;
}

void Exec_SBC( cpu6502 &cpu, uint16_t src )
{

}

void Exec_STA( cpu6502 &cpu, uint16_t src )
{

}
