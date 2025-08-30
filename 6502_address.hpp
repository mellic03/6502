// #pragma once

// #include "6502.hpp"


// uint16_t Addr_ACC( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }


// uint16_t Addr_ABSX( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }

// uint16_t Addr_ABSY( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }

// uint16_t Addr_IMM( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }

// uint16_t Addr_IMP( cpu6502 &cpu, uint16_t addr )
// {
//     return cpu.PC;
// }

// uint16_t Addr_INDX( cpu6502 &cpu, uint16_t addr )
// {
//     uint16_t lo = (cpu.Read(addr) + cpu.XR) & 0xFF;
//     uint16_t hi = (lo + 1) & 0xFF;
//     return cpu.Read(lo) + (cpu.Read(hi) << 8);
// }

// uint16_t Addr_INDY( cpu6502 &cpu, uint16_t addr )
// {
//     uint16_t lo, hi;
//     lo = cpu.Read(cpu.PC++);
//     hi = (lo + 1) & 0xFF;
//     return cpu.Read(lo) + (cpu.Read(hi) << 8) + cpu.YR;
// }

// uint16_t Addr_REL( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }

// uint16_t Addr_ZPG( cpu6502 &cpu, uint16_t addr )
// {

//     return cpu.Read(addr);
//     return 0;
// }

// uint16_t Addr_ZPGX( cpu6502 &cpu, uint16_t addr )
// {
//     cpu.Read(addr + cpu.XR)

//     return 0;
// }

// uint16_t Addr_ZPGY( cpu6502 &cpu, uint16_t addr )
// {
//     return 0;
// }







// // template <typename T>
// // struct Addressor
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return (T())(cpu, addr);
// //     }
// // };


// // struct AdACC: Addressor<AdACC>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };


// // struct AdABSX: Addressor<AdABSX>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdABSY: Addressor<AdABSY>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdIMM: Addressor<AdIMM>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return cpu.PC;
// //     }
// // };

// // struct AdIMP: Addressor<AdIMP>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdINDX: Addressor<AdINDX>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         uint16_t lo = (cpu.Read(addr) + cpu.XR) & 0xFF;
// //         uint16_t hi = (lo + 1) & 0xFF;
// //         return cpu.Read(lo) + (cpu.Read(hi) << 8);
// //     }
// // };

// // struct AdINDY: Addressor<AdINDY>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdREL: Addressor<AdREL>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdZPG: Addressor<AdZPG>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdZPGX: Addressor<AdZPGX>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };

// // struct AdZPGY: Addressor<AdZPGY>
// // {   uint16_t operator()( cpu6502 &cpu, uint16_t addr )
// //     {
// //         return 0;
// //     }
// // };
