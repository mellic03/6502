#pragma once

#include "../6502/6502.hpp"
#include "../system/bus.hpp"
#include "../system/clock.hpp"
#include "./ppu.hpp"





struct NesEmu
{
    using BusAttachmentCPU = cpu6502;

    SignalEmitter    hwtimer;
    DataBus6502      bus6502;
    DataBusPPU       busPPU;
    BusAttachmentCPU cpu;
    BusAttachmentPPU ppu;

    NesEmu()
    // :   hwtimer(1'790'000),
    :   hwtimer(50),
        cpu(&bus6502), ppu(&busPPU)
    {
        cpu.Listen(hwtimer);
        ppu.Listen(hwtimer);
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


