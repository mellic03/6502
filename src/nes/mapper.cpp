#include "mapper.hpp"

#include <string.h>
#include "ines.hpp"
#include "nes.hpp"

/*
    CPU $6000-$7FFF:
        Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB
        window, write protectable with an external switch.

    CPU $8000-$BFFF:
        First 16 KB of ROM.

    CPU $C000-$FFFF:
        Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
*/

void Mapper0( NesEmu *nes )
{
    // PRG RAM
    nes->cpu_bus.map(0x6000, 0x7FFF,
        [=](uint16_t x) { return nes->cpu_ram[(x - 0x6000) % 2048]; },
        [=](uint16_t x, uint8_t v) { nes->cpu_ram[(x - 0x6000) % 2048] = v; }
    );

    // First 16KB of ROM
    nes->cpu_bus.map(0x8000, 0xBFFF,
        [=](uint16_t x) { return nes->cpu_rom[x-0x8000]; },
        [=](uint16_t x, uint8_t v) { nes->cpu_rom[x-0x8000] = v; }
    );

    // NROM-128, mirror of 0x8000-0xBFFF
    nes->cpu_bus.map(0xC000, 0xFFFF,
        [=](uint16_t x) { return nes->cpu_rom[x-0xC000]; },
        [=](uint16_t x, uint8_t v) { nes->cpu_rom[x-0xC000] = v; }
    );

    // // NROM-256, second 16KB of ROM.
    // nes->cpu_bus.map(0xC000, 0xFFFF,
    //     [=](uint16_t x) { return nes->cpu_rom[16*1024 + (x-0xC000)]; },
    //     [=](uint16_t x, uint8_t v) { nes->cpu_rom[16*1024 + (x-0xC000)] = v; }
    // );
}

