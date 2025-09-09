// #include "bus.hpp"
// #include <stdio.h>
// #include <string.h>
// #include <cassert>
// #include <cstdlib>


// #define ErrAssert(Cond, Msg, ...) \
//     if (!(Cond)) \
//     { \
//         fprintf(stderr, "" Msg "\n", ##__VA_ARGS__); \
//         fprintf(stderr, "(%s:%d)\n", __FILE__, __LINE__); \
//         assert((Cond)); \
//     }

// static uint16_t dummy_dc( uint16_t i ) { return i; }

// static uint8_t dummy_rd( uint16_t )
// { fprintf(stderr, "[DataBus::DataBus] Bad read\n"); exit(1); return 0; }

// static void dummy_wt( uint16_t, uint8_t )
// { fprintf(stderr, "[DataBus::DataBus] Bad write\n"); exit(1); }


// DataBus::DataBus()
// {
//     // mAddrMaps.push_back({0, 0});
//     memset(mPageTables, 0, sizeof(mPageTables));
//     // mPageFuncs.push_back({nullptr, nullptr, nullptr, nullptr});
// }

// void DataBus::tick( uint64_t dt )
// {
//     for (HwDevice *dev: mDevices)
//     {
//         dev->tick(dt);
//     }
// }


// void DataBus::attach( HwDevice *dev )
// {
//     dev->mBus = this;
//     mDevices.insert(dev);
// }

// //      8421
// // 0000 1000
// // 0000 0111

// static constexpr uint16_t base  = 0x0006;
// static constexpr uint16_t mask  = 0b0111;

// static constexpr uint16_t addr0  = 0x0007;
// static constexpr uint16_t addr1  = 0x0008;
// static constexpr uint16_t addr2  = 0x0009;
// static constexpr uint16_t addr3  = 0x000A;
// static constexpr uint16_t addr4  = 0x000B;
// static constexpr uint16_t addr5  = 0x000C;
// static constexpr uint16_t addr6  = 0x000D;
// static constexpr uint16_t addr7  = 0x000E;
// static constexpr uint16_t addr8  = 0x000F;
// static constexpr uint16_t addr9  = 0x0010;
// static constexpr uint16_t addr10 = 0x0011;
// static constexpr uint16_t addr11 = 0x0012;

// static constexpr uint16_t dcde0  = base + (addr0 & mask);
// static constexpr uint16_t dcde1  = base + (addr1 & mask);
// static constexpr uint16_t dcde2  = base + (addr2 & mask);
// static constexpr uint16_t dcde3  = base + (addr3 & mask);
// static constexpr uint16_t dcde4  = base + (addr4 & mask);
// static constexpr uint16_t dcde5  = base + (addr5 & mask);
// static constexpr uint16_t dcde6  = base + (addr6 & mask);
// static constexpr uint16_t dcde7  = base + (addr7 & mask);
// static constexpr uint16_t dcde8  = base + (addr8 & mask);
// static constexpr uint16_t dcde9  = base + (addr9 & mask);
// static constexpr uint16_t dcde10 = base + (addr10 & mask);
// static constexpr uint16_t dcde11 = base + (addr11 & mask);


// const uint8_t *DataBus::getReadPtr( uint16_t addr )
// {
//     auto pgNum  = addr/256;
//     auto pgAddr = 256*pgNum;
//     auto &entry = mPageTables[pgNum];

//     auto &[page, base, mask, rwx, in_use] = entry;
//     ErrAssert(in_use == true, "Cannot read page at 0x%04X: page not mapped\n", pgAddr);
//     ErrAssert((page != nullptr), "Cannot read page at 0x%04X: page does not exist\n", pgAddr);
//     ErrAssert((rwx & RWX::R), "[getReadPtr] Page at 0x%04X has no read flag\n", pgAddr);

//     return &page[(addr - base) & mask];
// }

// uint8_t *DataBus::getWritePtr( uint16_t addr )
// {
//     auto pgNum  = addr/256;
//     auto pgAddr = 256*pgNum;
//     auto &entry = mPageTables[pgNum];

//     auto &[page, base, mask, rwx, in_use] = entry;
//     ErrAssert(in_use == true, "Page at 0x%04X is not mapped\n", pgAddr);
//     ErrAssert((page != nullptr), "Page at 0x%04X does not exist\n", pgAddr);
//     ErrAssert((rwx & RWX::W), "[getWritePtr] Page at 0x%04X is not writable\n", pgAddr);

//     return &page[(addr - base) & mask];
// }


// void DataBus::mapPage( uint16_t addr, uint16_t mask, void *page, uint8_t rwx )
// {
//     auto &entry = mPageTables[addr >> 8];
//     ErrAssert(!entry.in_use, "Cannot map page at 0x%04X: page already mapped\n", addr);
//     entry = PageTableEntry(addr, mask, (uint8_t*)page, rwx);
// }


// void DataBus::mapRange( uint16_t base, uint16_t end, uint16_t mask, void *pages, uint8_t rwx )
// {
//     assert(base % 256 == 0);
//     uint8_t *pMem = (uint8_t*)pages;

//     for (size_t addr=base; addr<=end; addr+=256)
//     {
//         mapPage(addr, mask, pMem, rwx);
//         pMem += 256;
//     }
// }


// uint8_t DataBus::read( uint16_t addr )
// {
//     auto *ptr = getReadPtr(addr);
//     if (ptr) return *ptr;
//     else     return 0;
// }


// void DataBus::write( uint16_t addr, uint8_t byte )
// {
//     uint8_t *ptr = getWritePtr(addr);
//     if (ptr) *ptr = byte;
// }




