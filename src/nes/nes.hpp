#pragma once

#include "../6502/6502.hpp"
#include "../system/bus.hpp"
#include "../system/clock.hpp"
#include "./ppu.hpp"





struct NesEmu
{
    using BusBusDeviceCPU = cpu6502;

    SignalEmitter hwtimer;
    DataBus       busCPU;
    DataBus       busPPU;
    cpu6502       cpu;
    BusDevicePPU  ppu;

    NesEmu()
    // :   hwtimer(1'790'000)
    :   hwtimer(50)
    //    cpu(busCPU),
    //    ppu(busPPU),
    {
        cpu.Listen(hwtimer);
        ppu.Listen(hwtimer);

        // busCPU.attach(&(ppu.mRam), 0x2000, 0x3FFF, cpureadvram, cpuwritevram);

    }

    // SignalEmitter hwtimer(50);
    // DataBus6502 cpu_bus;
    // DataBusPPU  ppu_bus;

    // auto cpu = BusAttachment6502(&cpu_bus);
    // // cpu.LoadROM(rom);
    // cpu.Listen(hwtimer);

    // auto ppu = BusAttachmentPPU(&ppu_bus);
    // ppu.Listen(hwtimer);


};


