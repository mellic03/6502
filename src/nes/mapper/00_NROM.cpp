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

static void MapNROM128( DataBus &bus, MemoryRO *prg_rom )
{
    bus.map(prg_rom, 0xC000, 0xFFFF, // CPU --> ROM, NROM-128, mirror of 0x8000-0xBFFF
        DC_FUNC(          return addr - 0xC000; ),
        RD_FUNC(MemoryRO, return dev->rd(addr); ),
        WT_FUNC(MemoryRO, dev->wt(addr, byte);  )
    );
}

static void MapNROM256( DataBus &bus, MemoryRO *prg_rom )
{
    bus.map(prg_rom, 0xC000, 0xFFFF, // NROM-256, second 16KB of ROM.
        DC_FUNC(          return (addr - 0xC000) + 16*1024; ),
        RD_FUNC(MemoryRO, return dev->rd(addr); ),
        WT_FUNC(MemoryRO, dev->wt(addr, byte);  )
    );
}


void HwMapper00_NROM::map( System &nes, GamePak *cart )
{
    using namespace NesFile;

    auto &bus = nes.mBusCPU;
    auto *pak = nes.mGamePak;

    iNES *file   = pak->miNES;
    auto &info   = file->mInfo;
    auto *PrgROM = &(file->mPrgROM);

    bus.map(&(nes.wRAM), 0x0000, 0x2000, // CPU --> CPU working RAM.
        DC_FUNC(         return addr % 2048;    ),
        RD_FUNC(MemRW2K, return dev->rd(addr);  ),
        WT_FUNC(MemRW2K, dev->wt(addr, byte);   )
    );
    
    bus.map(&(nes.wRAM), 0x6000, 0x7FFF, // CPU --> PRG RAM
        DC_FUNC(         return addr % 2048;   ),
        RD_FUNC(MemRW2K, return dev->rd(addr); ),
        WT_FUNC(MemRW2K, dev->wt(addr, byte);  )
    );

    bus.map(PrgROM, 0x8000, 0xBFFF, // CPU --> ROM, First 16KB of ROM
        DC_FUNC(          return addr - 0x8000; ),
        RD_FUNC(MemoryRO, return dev->rd(addr); ),
        WT_FUNC(MemoryRO, dev->wt(addr, byte);  )
    );

    if (info.prgRomSz <= 16*1024)
        MapNROM128(bus, PrgROM);
    else
        MapNROM256(bus, PrgROM);
}


static constexpr uint16_t GetAddr( uint16_t addr )
{
    return 16*1024 + (addr - 0xC000);
}


void sddfssdfds()
{
    16388;

    static constexpr uint16_t ADDR = 0xC004;
    static constexpr uint16_t BBBB = ADDR - 0xC000;
    static constexpr uint16_t CCCC = 16*1024;

}