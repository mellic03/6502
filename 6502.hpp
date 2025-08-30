#pragma once

#include <stddef.h>
#include <stdint.h>
#include "6502_bus.hpp"


struct cpu6502RegisterPC
{
    uint8_t lo;
    uint8_t hi;
};

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


struct cpu6502
{
private:
    uint16_t _read16();

    /**
     *  N|7 V|6 B|5 4|4 D|3 I|2 Z|1 C|0
     **/ 
    template <uint8_t mask>
    void _setssr( uint16_t res );

    static constexpr uint16_t PAGE_ZERO  = 0*256;
    static constexpr uint16_t PAGE_STACK = 1*256;
    static constexpr uint16_t PAGE_PROG  = 2*256;
    

public:
    static constexpr uint16_t PAGE_SIZE  = 256;

    typedef uint8_t* (cpu6502::*GetAddrFn)();
    typedef void (cpu6502::*ExecFn)(uint8_t*);

    struct Inst
    {
        GetAddrFn fA;
        ExecFn fE;

        Inst( GetAddrFn fa = nullptr, ExecFn fe = nullptr )
        :   fA(fa), fE(fe) {  }

        void exec( cpu6502 &cpu )
        {
            (cpu.*fE)((cpu.*fA)());
        }
    };

    Inst     ftab[256];
    uint8_t  mInvalidOp;
    uint8_t  mCurrOp;
    cpu6502Bus mBus;
    // union {
    //     uint8_t  mMem[65535];
    //     uint16_t mMem16[65535/2 + 1];
    // };

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

    cpu6502();
    void LoadROM( uint8_t *rom );
    void WriteResetVector( uint16_t addr );
    void Run();

    uint8_t advance()
    {
        return mBus[PC++];
    }

    uint8_t read08( uint16_t addr )
    {
        return mBus[addr];
    }

    uint16_t read16( uint16_t addr )
    {
        return *(uint16_t*)(&mBus[addr]);
        // uint16_t lo = (uint16_t)(mMem[addr+0]);
        // uint16_t hi = (uint16_t)(mMem[addr+1]);
        // return (hi<<8) + lo;
    }

    void write16( uint16_t addr, uint16_t word )
    {
        *(uint16_t*)(&mBus[addr]) = word;
        // uint16_t lo = value & 0x00FF;
        // uint16_t hi = value >> 8;
        // mMem[addr+0] = lo;
        // mMem[addr+1] = hi;
    }

    void push08( uint8_t byte )
    {
        mBus[PAGE_STACK + SP] = byte;
        SP += 1;
    }

    uint8_t pop08()
    {
        SP -= 1;
        return mBus[PAGE_STACK + SP];
    }

    void push16( uint16_t word )
    {
        push08(uint8_t(0x00FF & word)); // lo
        push08(uint8_t(word >> 8));     // hi
    }

    uint16_t pop16()
    {
        return (pop08()<<8) + pop08();
    }


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
    void InstrORA( uint8_t* );
    void InstrPHA( uint8_t* );
    void InstrPHP( uint8_t* );
    void InstrPLA( uint8_t* );
    void InstrPLP( uint8_t* );
    void InstrSBC( uint8_t* );
    void InstrSTA( uint8_t* );

};
