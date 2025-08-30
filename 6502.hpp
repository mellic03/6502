#pragma once
#pragma once

#include <stddef.h>
#include <stdint.h>


struct cpu6502AddrSpace
{
    uint8_t *txt, *stk, *ram;
    cpu6502AddrSpace( uint8_t *text, uint8_t *stack, uint8_t *data )
    :   txt(text), stk(stack), ram(data) {  }
};

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
    uint8_t 💩 :1;

    /** break */
    uint8_t B :1;

    /** overflow */
    uint8_t V :1;

    /** negative result */
    uint8_t N :1;

    cpu6502RegisterSSR()
    :   C(0), Z(0), I(1), D(0), 💩(0), B(0), V(0), N(0) {  };
};


struct cpu6502
{
private:
    uint16_t _read16();

public:
    static constexpr uint16_t PAGE_ZERO  = 0*256;
    static constexpr uint16_t PAGE_STACK = 1*256;
    static constexpr uint16_t PAGE_PROG  = 2*256;

    typedef uint16_t (cpu6502::*GetAddrFn)();
    typedef void (cpu6502::*ExecFn)(uint16_t);

    struct Inst
    {
        GetAddrFn fA;
        ExecFn fE;

        Inst( GetAddrFn fa = nullptr, ExecFn fe = nullptr )
        :   fA(fa), fE(fe) {  }

        void exec( cpu6502 &cpu )
        {
            uint16_t addr = (cpu.*fA)();
            (cpu.*fE)(addr);
        }
    };
    Inst ftab[256];

    uint8_t  mInvalidOp;
    uint8_t  mCurrOp;
    uint8_t  mMem[65535];

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
    void LoadProgram( uint8_t *program, uint16_t size );
    void Run();


    uint8_t read08( uint16_t addr )
    {
        return mMem[addr];
    }

    uint16_t read16( uint16_t addr )
    {
        uint16_t lo = (uint16_t)mMem[addr+0]; 
        uint16_t hi = (uint16_t)mMem[addr+1];
        return (hi<<8) + lo;
    }


    void push08( uint8_t byte )
    {
        mMem[PAGE_STACK + SP] = byte;
        SP += 1;
    }

    uint8_t pop08()
    {
        SP -= 1;
        return mMem[PAGE_STACK + SP];
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


    uint16_t LoadACC();
    uint16_t LoadABS();
    uint16_t LoadABSX();
    uint16_t LoadABSY();
    uint16_t LoadIMM();
    uint16_t LoadIMP();
    uint16_t LoadIND();
    uint16_t LoadINDX();
    uint16_t LoadINDY();
    uint16_t LoadREL();
    uint16_t LoadZPG();
    uint16_t LoadZPGX();
    uint16_t LoadZPGY();

    void InstrADC( uint16_t src );
    void InstrAND( uint16_t src );
    void InstrASL( uint16_t src );

    void InstrBCC( uint16_t src );
    void InstrBCS( uint16_t src );
    void InstrBEQ( uint16_t src );
    void InstrBIT( uint16_t src );
    void InstrBNI( uint16_t src );
    void InstrBNE( uint16_t src );
    void InstrBPL( uint16_t src );
    void InstrBRK( uint16_t src );
    void InstrBVC( uint16_t src );
    void InstrBVS( uint16_t src );

    void InstrCMP( uint16_t src );
    void InstrEOR( uint16_t src );

    void InstrJMP( uint16_t src );
    void InstrJSR( uint16_t src );

    void InstrLDA( uint16_t src );
    void InstrLDX( uint16_t src );
    void InstrLDY( uint16_t src );
    void InstrORA( uint16_t src );
    void InstrSBC( uint16_t src );
    void InstrSTA( uint16_t src );

};
