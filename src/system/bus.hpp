#pragma once

#include <stddef.h>
#include <stdint.h>



class iDataBus
{
public:
    uint8_t *mMem;
    iDataBus( void *mem ): mMem((uint8_t*)mem) {  }
};


class DataBus: public iDataBus
{
public:
    DataBus( uint16_t maxaddr )
    :   iDataBus(new uint8_t[size_t(maxaddr) + 1]) {  }
};



class BusInterface
{
protected:
    uint16_t deplex( uint16_t addr ) { return addr; }

public:
    iDataBus *mBus;
    int8_t    mReadError;
    int8_t    mWriteError;

    BusInterface( iDataBus *bus )
    :   mBus(bus), mReadError(0), mWriteError(0) {  };

    uint8_t  rdbus( uint16_t addr );
    uint16_t rdbus16( uint16_t addr );
    void     wtbus( uint16_t addr, uint8_t byte );
    void     wtbus16( uint16_t addr, uint16_t word );

    uint8_t &operator[]( uint16_t addr )
    {
        return mBus->mMem[addr];
    }
};



template <typename BusInterfaceType>
class BusAttachment
{
public:
    BusInterfaceType mBus;

    BusAttachment( iDataBus *bus ): mBus(bus) {  };

    uint8_t  rdbus   ( uint16_t a ) { return mBus.rdbus(a);   }
    uint16_t rdbus16 ( uint16_t a ) { return mBus.rdbus16(a); }
    void     wtbus   ( uint16_t a, uint8_t  b ) { mBus.wtbus(a, b);   }
    void     wtbus16 ( uint16_t a, uint16_t w ) { mBus.wtbus16(a, w); }

    uint8_t &operator[]( uint16_t a ) { return mBus[a]; }
};

