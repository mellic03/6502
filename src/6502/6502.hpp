#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../system/bus.hpp"
#include "../system/clock.hpp"


struct cpu6502RegisterSSR
{
    /** carry; */
    uint8_t C :1;

    /** zero result; */
    uint8_t Z :1;

    /** interrupt disable; */
    uint8_t I :1;

    /** decimal mode */
    uint8_t D :1;

    /** unused */
    uint8_t _ :1;

    /** break */
    uint8_t B :1;

    /** overflow */
    uint8_t V :1;

    /** negative result */
    uint8_t N :1;

    cpu6502RegisterSSR()
    :   C(0), Z(0), I(1), D(0), _(0), B(0), V(0), N(0) {  };
} __attribute__((packed));



struct MMap6502
{
    // union {
    //     uint8_t ram[0x0800];
    //     struct {
    //         uint8_t zpage[0x0100]; // 0x0000 -> 0x0100
    //         uint8_t stack[0x0100]; // 0x0100 -> 0x0200
    //     };
    // };
    // uint8_t ram     [0x0800]; // 0x0000 -> 0x07FF

    union {
        uint8_t ram [0x0800];       // 0x0000 -> 0x07FF
        struct {
            uint8_t zpage[0x0100]; // 0x0000 -> 0x0100
            uint8_t stack[0x0100]; // 0x0100 -> 0x0200
        };
    };

 // uint8_t ram_mr1 [0x0800];   // 0x0800 -> 0x0FFF
 // uint8_t ram_mr2 [0x0800];   // 0x1000 -> 0x17FF
 // uint8_t ram_mr2 [0x0800];   // 0x1800 -> 0x1FFF
                                // 
    uint8_t ppu     [0x0008];   // 0x2000 -> 0x2008
 // uint8_t ppu_mr1 [0x1FF8];   // 0x2008 -> 0x3FFF
                                // 
    uint8_t apu_io  [0x0018];   // 0x4000 -> 0x4017
    uint8_t apu_io_ [0x0008];   // 0x4018 -> 0x401F
                                // 
    uint8_t umapped [0xBFE0];   // 0x4020 -> 0xFFFF,   Available for cartridge use.
                                // 0x6000 -> 0x7FFF,   Usually cartridge RAM, when present.
                                // 0x8000 -> 0xFFFF,   Usually cartridge ROM and mapper registers.

};



class DataBus6502: public iDataBus
{
private:

public:
    DataBus6502(): iDataBus(new uint8_t[0xFFFF+1]) {  }

};


class BusInterface6502: public BusInterface
{
private:
    uint16_t deplex( uint16_t addr )
    {
        if (0x0000<=addr && addr<=0x1FFF)
        {
            return addr % 0x0800;
        }

        if (0x2000<=addr && addr<=0x3FFF)
        {
            return 0x2000 + (addr % 8);
        }

        return addr;
    }
public:
    BusInterface6502( iDataBus *bus )
    :   BusInterface(bus) {  }
};



struct cpu6502: public BusAttachment<BusInterface6502>, public SignalListener
{
public:
    uint8_t  mInvalidOp;
    uint8_t  mCurrOp;
    size_t   mCycles;

    uint8_t  AC;
    uint8_t  XR;
    uint8_t  YR;
    uint8_t  SP;

    union {
        uint16_t PC;
        struct {
            uint8_t PC_lo;
            uint8_t PC_hi;
        };
    };

    union {
        uint8_t SSR_byte;
        cpu6502RegisterSSR SSR;
    };


    cpu6502( iDataBus *bus );
    void LoadROM( uint8_t *rom );
    virtual void Tick() final;


private:
    static constexpr uint16_t PAGE_ZERO  = 0*256;
    static constexpr uint16_t PAGE_STACK = 1*256;
    static constexpr uint16_t PAGE_PROG  = 2*256;

    // -----------------------------------------------------------------------------------------
    typedef uint8_t* (cpu6502::*GetAddrFn)();
    typedef void (cpu6502::*ExecFn)(uint8_t*);

    struct Inst
    {
        const char *label;
        ExecFn    fE;
        GetAddrFn fA;
        int       nCycles;

        Inst( const char *str, ExecFn fe, GetAddrFn fa, int ncyc )
        :   label(str), fE(fe), fA(fa), nCycles(ncyc) {  }

        Inst(): Inst("", nullptr, nullptr, 1) {  }
    
        void operator()( cpu6502 &cpu )
        {
            (cpu.*fE)((cpu.*fA)());
        }
    };

    Inst mFtab[256];
    // -----------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------------
    /** N|7 V|6 B|5 4|4 D|3 I|2 Z|1 C|0 **/ 
    template <uint8_t mask>
    void _setssr( uint16_t res );

    uint8_t  _adv08();
    uint16_t _adv16();
    // -----------------------------------------------------------------------------------------

    void     push08( uint8_t );
    void     push16( uint16_t );
    uint8_t  pop08();
    uint16_t pop16();

    uint8_t *LoadACC();
    uint8_t *LoadABS();
    uint8_t *LoadABSX();
    uint8_t *LoadABSY();
    uint8_t *LoadIMM();
    uint8_t *LoadIMP();
    uint8_t *LoadIND();
    uint8_t *LoadINDX();
    uint8_t *LoadINDY();
    uint8_t *LoadREL();
    uint8_t *LoadZPG();
    uint8_t *LoadZPGX();
    uint8_t *LoadZPGY();

    void InstrUnimp( uint8_t* );
    void InstrADC( uint8_t* );
    void InstrAND( uint8_t* );
    void InstrASL( uint8_t* );
    void InstrBCC( uint8_t* );
    void InstrBCS( uint8_t* );
    void InstrBEQ( uint8_t* );
    void InstrBIT( uint8_t* );
    void InstrBMI( uint8_t* );
    void InstrBNE( uint8_t* );
    void InstrBPL( uint8_t* );
    void InstrBRK( uint8_t* );
    void InstrBVC( uint8_t* );
    void InstrBVS( uint8_t* );
    void InstrCLC( uint8_t* );
    void InstrCLD( uint8_t* );
    void InstrCLI( uint8_t* );
    void InstrCLV( uint8_t* );
    void InstrCMP( uint8_t* );
    void InstrCPX( uint8_t* );
    void InstrCPY( uint8_t* );
    void InstrEOR( uint8_t* );
    void InstrJMP( uint8_t* );
    void InstrJSR( uint8_t* );
    void InstrLDA( uint8_t* );
    void InstrLDX( uint8_t* );
    void InstrLDY( uint8_t* );
    void InstrNOP( uint8_t* );
    void InstrORA( uint8_t* );
    void InstrPHA( uint8_t* );
    void InstrPHP( uint8_t* );
    void InstrPLA( uint8_t* );
    void InstrPLP( uint8_t* );
    void InstrRTI( uint8_t* );
    void InstrRTS( uint8_t* );
    void InstrSBC( uint8_t* );
    void InstrSEC( uint8_t* );
    void InstrSTA( uint8_t* );
    void InstrSTX( uint8_t* );
    void InstrSTY( uint8_t* );

};
