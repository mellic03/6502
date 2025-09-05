#pragma once

#include "bus.hpp"


/**
 * Read/write memory.
 */
class MemoryRW: public iBusDevice
{
private:
    uint8_t *mBack;
    uint8_t *mFront;

public:
    const uint16_t mSize;

    MemoryRW( uint32_t sz )
    : mSize(sz), mBack(new uint8_t[sz]), mFront(new uint8_t[sz]) {  };

    virtual uint8_t read( uint16_t );
    virtual void write( uint16_t, uint8_t );
    virtual void Tick() override;
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void Tick() final {  };
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual uint8_t read( uint16_t ) final { return 0; };
    virtual void Tick() final {  };
};

