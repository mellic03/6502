#pragma once
#include <memu/types.hpp>
#include <memu/signal.hpp>


namespace RP2C02_detail
{
    struct BaseHw;
};


#define WORD_LOHI(Nm) \
union { uword Nm; struct {ubyte Nm##_lo, Nm##_hi; }; }


struct RP2C02_detail::BaseHw
{
public:
    struct NTable
    {
        ubyte tile[30][32];
        ubyte attr[8][8];
    };

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
        ubyte oamaddr; // OAM read/write address
        ubyte oamdata; // OAM read/write data
        ubyte SCROLL;  // 
        WORD_LOHI(ppuaddr);
        ubyte ppudata; // VRAM read/write data
    };


    BaseHw()
    {
        // assert((ioCLK && ioINT));
    }

};


#undef WORD_LOHI
