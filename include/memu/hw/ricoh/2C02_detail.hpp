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
    union ppuctl_t {
        ubyte byte;
        struct {
            ubyte NameTabSel     :2;
            ubyte Increment      :1;
            ubyte SpriteTileSel  :1;
            ubyte BgTileSel      :1;
            ubyte SpriteHeight   :1;
            ubyte MasterSlaveSel :1;
            ubyte NMIEnabled     :1;
        } __attribute__((packed));
    };

    union ppumask_t {
        ubyte byte;
        struct {
            uint8_t GreyScale      :1; // greyscale
            uint8_t BackGndLeftCol :1; // background left column enable
            uint8_t SpriteLeftCol  :1; // sprite left column enable
            uint8_t BackGndEnable  :1; // background enable
            uint8_t SpriteEnable   :1; // sprite enable
            uint8_t BGR            :3; // colour emphasis
        } __attribute__((packed));
    };

    union ppustat_t {
        ubyte byte;
        struct {
            uint8_t Unused         :5;  // unused
            uint8_t SpriteOverflow :1;  // sprite overflow
            uint8_t SpriteHit      :1;  // sprite 0 hit
            uint8_t VBlank         :1;  // vblank
        } __attribute__((packed));
    };

    struct {
        ppuctl_t  ppuctl;
        ppumask_t ppumask;
        ppustat_t ppustat;
        uint8_t   OAMADDR; // OAM read/write address
        uint8_t   OAMDATA; // OAM read/write data
        uint8_t   SCROLL;  // 
        uint8_t   ADDR;    // VRAM read/write address (two writes: MSB then LSB)
        uint8_t   DATA;    // VRAM read/write data
    } __attribute__((packed));

    struct { // PinOut
        ubyte *ioCLK;
        ubyte *ioINT;
    };


    BaseHw()
    {
        // assert((ioCLK && ioINT));
    }

};
