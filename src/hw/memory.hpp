#pragma once

// #include "bus.hpp"
#include "device.hpp"
#include <cassert>


/**
 * Read/write memory.
 */
class MemoryRW: public ioDevice
{
public:
    MemoryRW(void *p, uint16_t s): ioDevice(p, s) {  };
    MemoryRW(): MemoryRW(nullptr, 0) {  };
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void ioWrite(uint16_t, ubyte) final;
    ubyte operator[](uint16_t i) { return mData[i]; }
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual ubyte ioRead(uint16_t) final;
};





template <uint16_t Xk>
struct MemoryXkRW: public MemoryRW
{
    MemoryXkRW(void *p): MemoryRW(p, Xk) {  };
    MemoryXkRW(): MemoryXkRW(new ubyte[Xk]) {  };
};

template <uint16_t Xk>
struct MemoryXkRO: public MemoryRO
{
    MemoryXkRO(void *p): MemoryRO(p, Xk) {  };
    MemoryXkRO(): MemoryXkRO(new ubyte[Xk]) {  };
};

template <uint16_t Xk>
struct MemoryXkWO: public MemoryWO
{
    MemoryXkWO(void *p): MemoryWO(p, Xk) {  };
    MemoryXkWO(): MemoryXkWO(new ubyte[Xk]) {  };
};

using Memory1pRW = MemoryXkRW<1*256>;
using Memory2pRW = MemoryXkRW<2*256>;
using Memory4pRW = MemoryXkRW<4*256>;

using Memory1kRW = MemoryXkRW<1*1024>;
using Memory2kRW = MemoryXkRW<2*1024>;
using Memory4kRW = MemoryXkRW<4*1024>;

using Memory1kRO = MemoryXkRO<1*1024>;
using Memory2kRO = MemoryXkRO<2*1024>;
using Memory4kRO = MemoryXkRO<4*1024>;

using Memory1kWO = MemoryXkWO<1*1024>;
using Memory2kWO = MemoryXkWO<2*1024>;
using Memory4kWO = MemoryXkWO<4*1024>;

