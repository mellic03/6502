#ifndef CPU_6502_H
#define CPU_6502_H

#include <stddef.h>
#include <stdint.h>

struct cpu_6502;
using fptr_6502 = void (*)( cpu_6502& );

struct ftable_6502
{
    fptr_6502 LOHI[16][16];

    ftable_6502();

    fptr_6502 get( uint8_t lo, uint8_t hi )
    {
        return LOHI[lo][hi];
    }
};




using cpu_6502_stack = uint8_t[256];

union cpu_6502_inst
{
    uint8_t byte;

    struct
    {
        uint8_t lo :4;
        uint8_t hi :4;
    };
};


struct cpu_6502_SSR
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
};


struct cpu_6502_PC
{
    uint8_t lo;
    uint8_t hi;
};


struct cpu_6502_registers
{
    uint8_t  AC;
    uint8_t  XR;
    uint8_t  YR;
    uint8_t  SP; // is actually 16, but hi byte is always 0x01

    union {
        uint8_t SSR_byte;
        cpu_6502_SSR SSR;
    };

    union {
        uint16_t PC_word;
        cpu_6502_PC PC;
    };

    cpu_6502_registers( uint8_t *prog )
    :   AC(*prog), XR(*prog), YR(*prog),
        SP(0x00)
    {
        this->SSR_byte = 0x00;
        this->PC_word = 0x0000;
    }
};



struct cpu_6502: public cpu_6502_registers
{
    ftable_6502   mFTab;
    cpu_6502_inst mInst;
    uint8_t      *mInstPtr;
    uint8_t      *mProg;
    uint8_t       mStk[256];
    uint8_t       mRAM[65535];
    bool          mRunning;

    cpu_6502( uint8_t *program );

    uint8_t *fetch();
    cpu_6502_inst decode( const uint8_t* );
    bool execute( const cpu_6502_inst& );


    void push08( uint8_t byte )
    {
        mStk[SP] = byte; SP+=1;
    }

    uint8_t pop08()
    {
        SP-=1; return mStk[SP];
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


};




#endif // CPU_6502_H
