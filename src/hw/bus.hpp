#pragma once

#include <stddef.h>
#include <stdint.h>
#include <set>
#include <vector>
#include <functional>


class DataBus;
class iBusDevice;
class iBusAddrMap;


class BusDevice
{
public:
    uint8_t (*read)(uint16_t);
    void    (*write)(uint16_t, uint8_t);
};


class DataBus
{
public:
    using ReadFn  = uint8_t* (*)(iBusDevice*, uint16_t base, uint16_t addr);
    using WriteFn = void (*)(iBusDevice*, uint16_t base, uint16_t, uint8_t);

private:
    using ReadFunc  = std::function<uint8_t(uint16_t addr)>;
    using WriteFunc = std::function<void(uint16_t addr, uint8_t byte)>;
    struct FPair { ReadFunc read; WriteFunc write; };
    std::vector<FPair> mPageFuncs;
    uint8_t mPageTable[0xFFFF + 1];
    std::set<iBusDevice*> mDevices;

public:
    DataBus();
    void map( uint16_t min, uint16_t max, ReadFunc, WriteFunc );
    void attach( iBusDevice* );
    void tick();

    uint8_t read( uint16_t addr );
    void write( uint16_t addr, uint8_t byte );
};


class iBusDevice
{
public:
    virtual void Tick() = 0;
};


class MemoryDevice: public iBusDevice
{
public:
    uint8_t *mMem;
    const uint16_t MaxAddr;

    virtual void Tick() final {  }

    MemoryDevice( uint32_t size )
    :   mMem(new uint8_t[size]),
        MaxAddr(size-1) {  }

    uint8_t &operator[]( uint16_t i );
};



// class iDataBus
// {
// public:
//     uint8_t *mMem;
//     iDataBus( void *mem ): mMem((uint8_t*)mem) {  }
// };


// class DataBus: public iDataBus
// {
// public:
//     DataBus( uint16_t maxaddr )
//     :   iDataBus(new uint8_t[size_t(maxaddr) + 1]) {  }
// };



// class BusInterface
// {
// protected:
//     virtual uint16_t deplex( uint16_t addr ) = 0;

// public:
//     iDataBus *mBus;
//     int8_t    mReadError;
//     int8_t    mWriteError;

//     BusInterface( iDataBus *bus )
//     :   mBus(bus), mReadError(0), mWriteError(0) {  };

//     uint8_t  rdbus( uint16_t addr );
//     uint16_t rdbus16( uint16_t addr );
//     void     wtbus( uint16_t addr, uint8_t byte );
//     void     wtbus16( uint16_t addr, uint16_t word );

//     uint8_t &operator[]( uint16_t addr )
//     {
//         return mBus->mMem[addr];
//     }
// };



// template <typename BusInterfaceType>
// class BusAttachment
// {
// public:
//     BusInterfaceType mBus;

//     BusAttachment( iDataBus *bus ): mBus(bus) {  };

//     uint8_t  rdbus   ( uint16_t a ) { return mBus.rdbus(a);   }
//     uint16_t rdbus16 ( uint16_t a ) { return mBus.rdbus16(a); }
//     void     wtbus   ( uint16_t a, uint8_t  b ) { mBus.wtbus(a, b);   }
//     void     wtbus16 ( uint16_t a, uint16_t w ) { mBus.wtbus16(a, w); }

//     uint8_t &operator[]( uint16_t a ) { return mBus[a]; }
// };

