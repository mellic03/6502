#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>


class DataBus;
class iBusDevice;
class iBusAddrMap;


class DataBus
{
public:
    using ReadFn = uint8_t* (*)(iBusDevice*, uint16_t base, uint16_t addr);
    using WriteFn = void (*)(iBusDevice*, uint16_t base, uint16_t, uint8_t);

private:
    struct Tup {
        iBusDevice *dev;
        uint16_t    base;
        ReadFn      rdfn;
        WriteFn     wtfn;
    };
    std::vector<Tup> mDevices;
    uint8_t          mDeviceMap[0xFFFF+1];
    uint8_t          mSafetyWord[4];

    uint8_t *_read( uint16_t addr );

public:
    DataBus();

    void attach( iBusDevice *dev, uint16_t minaddr, uint16_t maxaddr,
                 ReadFn rd, WriteFn wt );

    uint8_t read( uint16_t addr );
    uint16_t read16( uint16_t addr );
    void write( uint16_t addr, uint8_t byte );
    void write16( uint16_t addr, uint16_t word );

    uint8_t &operator[]( uint16_t i )
    {
        return *_read(i);
    }
};


class iBusDevice
{
protected:
    friend class DataBus;
    virtual uint8_t _iBus_read( uint16_t a ) { return 0; }
    virtual void _iBus_write( uint16_t a, uint8_t x ) {  }

public:

};


struct iBusAddrMap
{
private:

public:
    uint16_t mMin, mMax;
    iBusAddrMap( uint16_t min, uint16_t max ): mMin(min), mMax(max) {  }
    virtual uint16_t rmap( uint16_t x ) { return x; };
    virtual uint16_t wmap( uint16_t x ) { return x; };
};



// template <uint32_t Size>
class MemoryDevice: public iBusDevice
{
private:
    // uint8_t mMem[Size];
    virtual uint8_t _iBus_read( uint16_t a ) { return mMem[a]; }
    virtual void _iBus_write( uint16_t a, uint8_t x ) { mMem[a] = x; }

public:
    uint8_t *mMem;
    const uint16_t MaxAddr;

    MemoryDevice( uint32_t size )
    :   mMem(new uint8_t[size]),
        MaxAddr(size-1)
    {

    }

    uint8_t &operator[]( uint16_t i )
    {
        return mMem[i];
    }
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

