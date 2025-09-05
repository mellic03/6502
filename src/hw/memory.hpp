// #pragma once

// #include <stdint.h>
// #include <cassert>
// #include <vector>


// class iBus;
// class iBusDevice;
// class iBusAddrMap;


// class iBus
// {
// private:
//     struct dpair {
//         iBusDevice  *dev;
//         iBusAddrMap *map;
//     };
//     std::vector<dpair> mDevices;

// public:
//     void attach( iBusDevice *dev, iBusAddrMap *map )
//     {
//         mDevices.push_back({dev, map});
//     }

//     uint16_t read( uint16_t addr );
//     void write( uint16_t addr, uint8_t byte );
// };


// class iBusDevice
// {
// protected:

// public:

// };



// template <uint32_t Size>
// class MemoryDevice: public iBusDevice
// {
// private:
//     uint8_t mMem[Size];
//     // uint8_t *mMem;
// public:
//     // MemoryDevice( uint32_t size ): mMem(new uint8_t[size]) {  }
//     uint8_t &operator[]( uint16_t i )
//     {
//         assert((0<=i && i<Size));
//         return mMem[i];
//     }
// };

// // struct Dev6502: public iBusDevice {  };
// // struct DevPPU: public iBusDevice {  };

// // void sfsdesf()
// // {
// //     BusAddrMap<Dev6502, MemoryDevice> cpu_ram_map;
// //     BusAddrMap<Dev6502, DevPPU>       cpu_ppu_map;

// //     Dev6502 cpu;  MemoryDevice cpu_ram(2048);
// //     DevPPU  ppu;  MemoryDevice ppu_ram(2048);

// //     {
// //         iBus cpu_bus;
// //         cpu_ram.attach(&cpu_bus, &cpu_ram_map);
// //     }


// //     {
// //         iBus ppu_bus;

// //         ppu_ram.attach(&ppu_bus, &cpu_ppu_map);
// //     }
    

// //     // BusAddrMapMap<
// // }
