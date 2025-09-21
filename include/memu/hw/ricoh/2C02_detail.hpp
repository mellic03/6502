#pragma once
#include <memu/types.hpp>
#include <memu/signal.hpp>


namespace RP2C02_detail
{
    struct BaseHw;
};


struct RP2C02_detail::BaseHw
{
public:
    BaseHw()
    :   mPpuAddr(0), mPpuData(0) {  }

    union ppuctl_t {
        ubyte byte;
        struct {
            uint8_t NN :2; // X/Y Scroll
            uint8_t I  :1; // IncrementMode
            uint8_t S  :1; // SpriteTileSelect
            uint8_t B  :1; // BgTileSelect
            uint8_t H  :1; // SpriteHeight
            uint8_t P  :1; // MstrSlaveSelect
            uint8_t V  :1; // NMI enable
        } __attribute__((packed));
    };

    union ppumask_t {
        ubyte byte;
        struct {
            uint8_t G   :1; // greyscale
            uint8_t m   :1; // background left column enable
            uint8_t M   :1; // sprite left column enable
            uint8_t b   :1; // background enable
            uint8_t s   :1; // sprite enable
            uint8_t BGR :3; // colour emphasis
        } __attribute__((packed));
    };

    union ppustat_t {
        ubyte byte;
        struct {
            uint8_t U :5;  // unused
            uint8_t O :1;  // sprite overflow
            uint8_t S :1;  // sprite 0 hit
            uint8_t V :1;  // vblank
        } __attribute__((packed));
    };

    union {
        uint8_t MMIO[8];
        struct {
            ppuctl_t  CTRL;
            ppumask_t MASK;
            ppustat_t STATUS;
            uint8_t   OAMADDR; // OAM read/write address
            uint8_t   OAMDATA; // OAM read/write data
            uint8_t   SCROLL;  // 
            uint8_t   ADDR;    // VRAM read/write address (two writes: MSB then LSB)
            uint8_t   DATA;    // VRAM read/write data
        } __attribute__((packed));
    };

    // class DataLatch
    // {
    // private:
    //     bool mLatch = false;

    // public:
    //     union {
    //         uint16_t value;
    //         struct {
    //             uint8_t lo;
    //             uint8_t hi;
    //         };
    //     };

    //     void reset()
    //     {
    //         mLatch = false;
    //     }

    //     void write( uint8_t v )
    //     {
    //         if (mLatch) hi = v;
    //         else        lo = v;
    //         mLatch = !mLatch;
    //     }
    // };

    // DataLatch mScrl;

    union {
        uint16_t mPpuAddr;
        struct  {
            uint8_t mPpuAddrLo;
            uint8_t mPpuAddrHi;
        };
    };

    uint8_t mPpuData;

};
