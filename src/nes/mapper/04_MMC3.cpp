// #include "types.hpp"
// using namespace NesEmu;


// /*
//    Banks
//    --------------------------------------------------------------------------------------------
//     x CPU 6000-7FFF: 8 KB PRG RAM bank (optional)

//     x CPU 8000-9FFF: 8 KB switchable PRG ROM bank
//         | C000-DFFF

//     x CPU A000-BFFF: 8 KB switchable PRG ROM bank

//     x CPU 8000-9FFF: 8 KB PRG ROM bank, fixed to the second-last bank
//         | C000-DFFF

//     x CPU E000-FFFF: 8 KB PRG ROM bank, fixed to the last bank

//     - PPU 0000-07FF: 2 KB switchable CHR bank
//         | 1000-17FF

//     - PPU 0800-0FFF: 2 KB switchable CHR bank
//         | 1800-1FFF

//     - PPU 1000-13FF: 1 KB switchable CHR bank
//         | 0000-03FF

//     - PPU 1400-17FF: 1 KB switchable CHR bank
//         | 0400-07FF

//     - PPU 1800-1BFF: 1 KB switchable CHR bank
//         | 0800-0BFF

//     - PPU 1C00-1FFF: 1 KB switchable CHR bank
//         | 0C00-0FFF

// */

// Mapper004_MMC3::Mapper004_MMC3( System &nes, GamePak *gpak )
// {
//     // auto *gpak = nes.mGPak;
//     // ubyte *prgRom = gpak->mPrgROM.data();
//     // ubyte *chrRom = gpak->mChrROM.data();

//     // {
//     //     auto &bus = nes.mBusCPU;
//     //     auto &cpu = nes.mCPU;

//     //     // CPU 6000-7FFF: 8 KB PRG RAM bank (optional).
//     //     bus.mapRdRange(0x6000, 0x7FFF, 0x7FFF-0x6000, prgRom);

//     //     // CPU (8000-9FFF || C000-DFFF): 8 KB switchable PRG ROM bank.
//     //     bus.mapRdRange(0x8000, 0x9FFF, 0x9FFF-0x8000, prgRom);
//     //     bus.mapRdRange(0xC000, 0xDFFF, 0x9FFF-0x8000, prgRom);

//     //     // CPU A000-BFFF: 8 KB switchable PRG ROM bank.
//     //     bus.mapRdRange(0xA000, 0xBFFF, 0xBFFF-0xA000, prgRom);
        
//     //     // CPU (8000-9FFF || C000-DFFF): 8k PrgROM bank, fixed to 2nd-last bank
//     //     bus.mapRdRange(0x8000, 0x9FFF, 0x9FFF-0x8000, prgRom);
//     //     bus.mapRdRange(0xC000, 0xDFFF, 0x9FFF-0x8000, prgRom);

//     //     // CPU E000-FFFF: 8 KB PRG ROM bank, fixed to the last bank
//     //     bus.mapRdRange(0xE000, 0xFFFF, 0xFFFF-0xE000, prgRom);
//     // }
//     // {
//     //     auto &bus = nes.mBusPPU;
//     //     auto &ppu = nes.mPPU;

//     //     // CPU 6000-7FFF: 8 KB PRG RAM bank (optional).
//     //     bus.mapRdRange(0x6000, 0x7FFF, 0x7FFF-0x6000, prgRom);

//     //     // CPU (8000-9FFF || C000-DFFF): 8 KB switchable PRG ROM bank.
//     //     bus.mapRdRange(0x8000, 0x9FFF, 0x9FFF-0x8000, prgRom);
//     //     bus.mapRdRange(0xC000, 0xDFFF, 0x9FFF-0x8000, prgRom);

//     //     // CPU A000-BFFF: 8 KB switchable PRG ROM bank.
//     //     bus.mapRdRange(0xA000, 0xBFFF, 0xBFFF-0xA000, prgRom);
        
//     //     // CPU (8000-9FFF || C000-DFFF): 8k PrgROM bank, fixed to 2nd-last bank
//     //     bus.mapRdRange(0x8000, 0x9FFF, 0x9FFF-0x8000, prgRom);
//     //     bus.mapRdRange(0xC000, 0xDFFF, 0x9FFF-0x8000, prgRom);

//     //     // CPU E000-FFFF: 8 KB PRG ROM bank, fixed to the last bank
//     //     bus.mapRdRange(0xE000, 0xFFFF, 0xFFFF-0xE000, prgRom);
//     // }

//     // // {
//     // //     auto &bus = nes.mBusPPU;
//     // //     auto &ppu = nes.mPPU;
//     // //     ubyte *ppuRam = ppu.mVRAM.data();

//     // //     bus.mapRdWtRange(0x2000, 0x23FF, 1024-1, ppuRam + 0*1024);
//     // //     bus.mapRdWtRange(0x2400, 0x27FF, 1024-1, ppuRam + 1*1024);

//     // //     bus.mapRdWtRange(0x2800, 0x2BFF, 1024-1, ppuRam + 0*1024);
//     // //     bus.mapRdWtRange(0x2C00, 0x2FFF, 1024-1, ppuRam + 1*1024);
//     // // }
// }

