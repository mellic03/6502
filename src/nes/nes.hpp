#pragma once

#include "../6502/6502.hpp"
#include "../hw/bus.hpp"
#include "./ppu.hpp"


struct NesEmu
{
    DataBus       cpu_bus;
    uint8_t      *cpu_ram;
    uint8_t      *cpu_rom;
    cpu6502       cpu;

    DataBus       ppu_bus;
    uint8_t      *ppu_vram;
    BusDevicePPU  ppu;

    NesEmu();
    // :   hwtimer(1'790'000)
    // :   hwtimer(50)
    //    cpu(busCPU),
    //    ppu(busPPU),
    // {
    //     // cpu.Listen(hwtimer);
    //     // ppu.Listen(hwtimer);

    //     // busCPU.attach(&(ppu.mRam), 0x2000, 0x3FFF, cpureadvram, cpuwritevram);

    // }

    void LoadROM( uint8_t *rom );

    // SignalEmitter hwtimer(50);
    // DataBus6502 cpu_bus;
    // DataBusPPU  ppu_bus;

    // auto cpu = BusAttachment6502(&cpu_bus);
    // // cpu.LoadROM(rom);
    // cpu.Listen(hwtimer);

    // auto ppu = BusAttachmentPPU(&ppu_bus);
    // ppu.Listen(hwtimer);


};


