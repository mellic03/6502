#include "ppu.hpp"
#include <stdio.h>


/*
    Register	        At Power    After Reset
    PPUCTRL             0000 0000	0000 0000
    PPUMASK             0000 0000	0000 0000
    PPUSTATUS           +0+x xxxx	U??x xxxx
    OAMADDR             $00	        unchanged
    $2005/$2006 latch	cleared     cleared
    PPUSCROLL           $0000	    $0000
    PPUADDR             $0000	    unchanged
    PPUDATA             read buffer	$00	$00

    ? = unknown, x = irrelevant, + = often set, U = unchanged
*/


/*
    - Horizontal arrangement: $2000 and $2800 contain the first nametable,
      and $2400 and $2C00 contain the second nametable (e.g. Super Mario Bros.),
      accomplished by connecting CIRAM A10 to PPU A10.

    - Vertical arrangement: $2000 and $2400 contain the first nametable,
      and $2800 and $2C00 contain the second nametable (e.g. Kid Icarus),
      accomplished by connecting CIRAM A10 to PPU A11.
*/

NesPPU::NesPPU( DataBus *bus )
:   HwDevice(bus)
{
    mMMIO   = (RegMMIO*)(mVRAM.data() + 0);
    mTables = (TableData*)(mVRAM.data() + sizeof(RegMMIO));

    // ubyte *VRAM = mVRAM.data();
    // mBus->mapRange(0x2000, 0x2FFF, 2048-1, &mTables[0]);

    // | Addr      | Size | Desc        | Mapped By |
    // | ----------|------|-------------|-----------|
    // | 0000-0FFF | 1000 | PtrnTable 0 | Cartridge |
    // | 1000-1FFF | 1000 | PtrnTable 1 | Cartridge |
    // | 2000-23BF | 03C0 | NameTable 0 | Cartridge |
    // | 23C0-23FF | 0040 | AttrTable 0 | Cartridge |
    // | 2400-27BF | 03c0 | Nametable 1 | Cartridge |
    // | 27C0-27FF | 0040 | AttrTable 1 | Cartridge |
    // | 2800-2BBF | 03c0 | Nametable 2 | Cartridge |
    // | 2BC0-2BFF | 0040 | AttrTable 2 | Cartridge |
    // | 2C00-2FBF | 03c0 | Nametable 3 | Cartridge |
    // | 2FC0-2FFF | 0040 | AttrTable 3 | Cartridge |
    // ----------------------------------------------

    /*
        - The NES has 2kB of RAM dedicated to the PPU, usually mapped to the
          nametable address space from $2000-$2FFF, but this can be rerouted
          through custom cartridge wiring. The mappings above are the addresses
          from which the PPU uses to fetch data during rendering. The actual
          devices that the PPU fetches pattern, name table and attribute table
          data from is configured by the cartridge.

        - 0000-1FFF normally mapped by the cartridge to a CHR-ROM or CHR-RAM,
          often with a bank switching mechanism.

        - 2000-2FFF normally mapped to the 2kB NES internal VRAM, providing 2
          nametables with a mirroring configuration controlled by the cartridge,
          but it can be partly or fully remapped to ROM or RAM on the cartridge,
          allowing up to 4 simultaneous nametables.

        - 3000-3EFF usually a mirror of the 2kB region from $2000-2EFF. The PPU
          does not render from this address range, so this space has negligible
          utility.

        - 3F00-3FFF not configurable, always mapped to the internal palette control.

    */

    // PPU --> NameTables
    // mBus->mapRange(0x2000, 0x23FF, 1024-1, &mTables[0]);
//     mBus->mapRange(0x2400, 0x27FF, 1024-1, &mTables[1]);
//     mBus->mapRange(0x2800, 0x2BFF, 1024-1, &mTables[2]);
//     mBus->mapRange(0x2C00, 0x2FFF, 1024-1, &mTables[3]);

}


void NesPPU::tick( uint64_t dt )
{
    // printf("[PPU]\n");
}

