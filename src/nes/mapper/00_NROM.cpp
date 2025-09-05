#include "hwmapper.hpp"
using namespace NesEmu;



/*
    - PRG ROM size: 16 KiB for NROM-128, 32 KiB for NROM-256 (DIP-28 standard pinout)
    - PRG ROM bank size: Not bankswitched
    - PRG RAM: 2 or 4 KiB, not bankswitched, only in Family Basic (but most emulators provide 8)
    - CHR capacity: 8 KiB ROM (DIP-28 standard pinout) but most emulators support RAM
    - CHR bank size: Not bankswitched, see CNROM
    - Nametable mirroring: Solder pads select vertical or horizontal mirroring
    - Subject to bus conflicts: Yes, but irrelevant
*/

/*
    CPU $6000-$7FFF:
        Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB
        window, write protectable with an external switch.

    CPU $8000-$BFFF:
        First 16 KB of ROM.

    CPU $C000-$FFFF:
        Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
*/
void HwMapper00_NROM::map( System &nes, Cartridge *cart )
{

    // 0x A    B    C    D
    // 0b 0000 0000 0000 0000
    union BusAddr
    {
        uint16_t word;

        struct
        {
            uint8_t D :4;
            uint8_t C :4;
            uint8_t B :4;
            uint8_t A :4;
        };
    };

    auto &bus_cpu = nes.cpu_bus;

    // PRG RAM
    bus_cpu.map(0x6000, 0x7FFF,
        [=](uint16_t x) { return nes.cpu_ram[(x - 0x6000) % 2048]; },
        [=](uint16_t x, uint8_t v) { nes.cpu_ram[(x - 0x6000) % 2048] = v; }
    );

    // First 16KB of ROM
    bus_cpu.map(0x8000, 0xBFFF,
        [=](uint16_t x) { return nes.cpu_rom[x-0x8000]; },
        [=](uint16_t x, uint8_t v) { nes.cpu_rom[x-0x8000] = v; }
    );

    // NROM-128, mirror of 0x8000-0xBFFF
    bus_cpu.map(0xC000, 0xFFFF,
        [=](uint16_t x) { return nes.cpu_rom[x-0xC000]; },
        [=](uint16_t x, uint8_t v) { nes.cpu_rom[x-0xC000] = v; }
    );

    // // NROM-256, second 16KB of ROM.
    // nes.cpu_bus.map(0xC000, 0xFFFF,
    //     [=](uint16_t x) { return nes.cpu_rom[16*1024 + (x-0xC000)]; },
    //     [=](uint16_t x, uint8_t v) { nes.cpu_rom[16*1024 + (x-0xC000)] = v; }
    // );
}

