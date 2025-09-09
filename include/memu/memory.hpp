#pragma once

#include "hwmodule.hpp"
#include "rwx.hpp"
#include <array>
#include <cassert>

// namespace memu
// {
//     class AddressableDevice;

//     template <size_t ByteSize>
//     class MemoryDevice;
// }

// class memu::AddressableDevice
// {
// private:

// public:
//     virtual bool isLoadAddr(addr_t) = 0;
//     virtual bool isStorAddr(addr_t) = 0;
//     virtual bool isValidAddr(addr_t) = 0;

//     // MemoryDevice(): MemoryDevice(nullptr, 0) {  };
//     // MemoryDevice(void *p, uint16_t s): mData(p), mSize(s) {  };
//     virtual ubyte read(addr_t) = 0;
//     virtual void write(addr_t, ubyte) = 0;

// };


// template <size_t SZ>
// class memu::MemoryDevice: public AddressableDevice
// {
// private:
//     std::array<ubyte, SZ> mData;

// public:
//     // MemoryDevice(): MemoryDevice(nullptr, 0) {  };
//     // MemoryDevice(void *p, uint16_t s): mData(p), mSize(s) {  };
//     virtual bool isValidAddr(addr_t i) override { return 0<=i && i<Sz; }
//     virtual bool isLoadAddr(addr_t i) override { return isValidAddr(i); }
//     virtual bool isStorAddr(addr_t i) override { return isValidAddr(i); }

//     virtual ubyte read(addr_t i) override
//     {
//         assert(isValidAddr(i));

//     }

//     virtual void write(addr_t i, ubyte x) override
//     {

//     }
// };



/**
 * Read/write memory.
 */
class MemoryRW // : public ioDevice
{
private:
    ubyte *mData;
    size_t mSize;

public:
    MemoryRW(): MemoryRW(nullptr, 0) {  };
    MemoryRW(void *p, uint16_t s): mData((ubyte*)p), mSize(s) {  };
    ubyte *data() { return mData; } 
    virtual ubyte read( addr_t );
    virtual void write( addr_t, ubyte );
};


/**
 * Read-only memory.
 */
class MemoryRO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual void write(addr_t, ubyte) final;
    ubyte operator[](addr_t i) { return this->read(i); }
};


/**
 * Write-only memory.
 */
class MemoryWO: public MemoryRW
{
public:
    using MemoryRW::MemoryRW;
    virtual ubyte read(addr_t) final;
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

