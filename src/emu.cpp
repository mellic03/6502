// #include <iostream>
// #include <fstream>

// #include <stdio.h>
// #include <assert.h>
// #include <stddef.h>
// #include <stdint.h>

// #include "6502/6502.hpp"
// #include "nes/ppu.hpp"
// #include "nes/ines.hpp"
// #include "nes/mapper.hpp"
// #include "nes/nes.hpp"
// #include "hw/display.hpp"


// namespace emu { int entry(uint8_t*); }



// // https://bugzmanov.github.io/nes_ebook/chapter_7.html

// int emu::entry( uint8_t *rom )
// {
//     NesEmu *nes = new NesEmu();
//     nes->LoadROM(rom);
//     nes->cpu.PC = 0x8000;

//     Display D;
//     D.init(256, 240, 4);

//     uint64_t tcurr = SDL_GetTicks64();
//     uint64_t tprev = tcurr;
//     uint64_t accum = 0;

//     while (!nes->cpu.mInvalidOp)
//     {
//         D.beginFrame();

//         tcurr  = SDL_GetTicks64();
//         accum += tcurr - tprev;
//         tprev  = tcurr;

//         if (accum >= 100)
//         {
//             accum = 0;
//             nes->cpu_bus.tick();
//         }

//         // uint8_t status = nes->cpu_bus.read(0x6000);
//         // printf("status: 0x%02X\n", status);

//         // char ch = '>';
//         // uint16_t addr = 0x6004;

//         // while (ch)
//         // {
//         //     printf("%c", ch);
//         //     ch = nes->cpu_bus.read(addr++);
//         // }
//         // printf("\n\n");

//         if (nes->cpu.mOpCount >= 500)
//         {
//             break;
//         }

//         if (D.keyReleased(SDL_SCANCODE_I))
//         {
//             printf("Key I --> IRQ\n");
//             nes->cpu.sig_irq();
//         }

//         if (D.keyReleased(SDL_SCANCODE_R))
//         {
//             printf("Key R --> RESET\n");
//             nes->cpu.sig_res();
//         }

//         if (D.keyReleased(SDL_SCANCODE_N))
//         {
//             printf("Key N --> NMI\n");
//             nes->cpu.sig_nmi();
//         }

//         if (D.keyReleased(SDL_SCANCODE_ESCAPE))
//         {
//             break;
//         }

//         D.endFrame();
//     }
//     return 0;
// }


