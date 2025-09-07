#pragma once

#include <stddef.h>
#include <stdint.h>
#include "../hw/bus.hpp"
#include "../hw/hwmodule.hpp"
#include "../hw/memory.hpp"

/**
 * - Break (B) Changes when SSR is pushed/popped. When pushed, it will
 *   be 1 when transfered by a BRK or PHP instruction, and zero otherwise
 *   (i.e., when pushed by a hardware interrupt). When pulled into the
 *   status register (by PLP or on RTI), it will be ignored.
 * 
 * - The break flag will be set to on (1), whenever the transfer was
 *   caused by software (BRK or PHP).
 * 
 * - The break flag will be set to zero (0), whenever the transfer was
 *   caused by a hardware interrupt.
 * 
 * - The break flag will be masked and cleared (0), whenever transferred
 *   from the stack to the status register, either by PLP or during a
 *   return from interrupt (RTI).
 *
 * - Interrupt Disable (I) When set, IRQ interrupts are inhibited. NMI, BRK,
 *   and reset are not affected.
 * - Can be set or cleared directly with SEI or CLI.
 * - Automatically set by the CPU after pushing flags to the stack when any
 *   interrupt is triggered (NMI, IRQ/BRK, or reset). Restored to its previous
 *   state from the stack when leaving an interrupt handler with RTI.
 * - If an IRQ is pending when this flag is cleared (i.e. the /IRQ line is low),
 *   an interrupt will be triggered immediately. However, the effect of toggling
 *   this flag is delayed 1 instruction when caused by SEI, CLI, or PLP.
 * 
 */
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
    uint8_t U :1;

    /** break */
    uint8_t B :1;

    /** overflow */
    uint8_t V :1;

    /** negative result */
    uint8_t N :1;

    cpu6502RegisterSSR()
    :   C(0), Z(0), I(1), D(0), U(0), B(1), V(0), N(0) {  };
} __attribute__((packed));



// https://www.nesdev.org/wiki/CPU_power_up_state
struct cpu6502: public Emu::HwModule
{
public:
    Memory2kRW mRAM;

    uint8_t  mInvalidOp;
    uint8_t  mCurrOp;
    size_t   mCycles;
    size_t   mOpCount;
    bool     mOpAC;
    uint16_t mOpAddr;
    uint8_t  mOpData;

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

    // cpu6502( iDataBus *bus );
    cpu6502( Emu::AddrSpace& );
    virtual void tick() final;
    virtual void reset() final;
    // ubyte rdbus( uint16_t i ) { return busRead(i); }
    // void  wtbus( uint16_t i, ubyte v ) { busWrite(i, v); };

    union HwPins
    {
        uint8_t byte;
        struct {
            uint8_t irq :1;
            uint8_t res :1;
            uint8_t nmi :1;
        };
        HwPins(): irq(1), res(1), nmi(0) {  };
    };

    bool m_wai = false;
    HwPins m_pins;
    HwPins m_pins_prev;

    void sig_irq() { m_pins.irq = 0; }
    void sig_res() { m_pins.res = 0; }
    void sig_nmi() { m_pins.nmi = !m_pins_prev.nmi; }


private:
    static constexpr uint16_t PAGE_ZERO  = 0*256;
    static constexpr uint16_t PAGE_STACK = 1*256;
    static constexpr uint16_t PAGE_PROG  = 2*256;

    // -----------------------------------------------------------------------------------------
    // typedef int (cpu6502::*GetAddrFn)(int);
    typedef int (cpu6502::*GetAddrFn)();
    typedef void (cpu6502::*ExecFn)();

    struct Inst
    {
        const char *label;
        ExecFn    fE;
        GetAddrFn fA;
        int       nCycles;

        Inst( const char *str, ExecFn fe, GetAddrFn fa, int ncyc )
        :   label(str), fE(fe), fA(fa), nCycles(ncyc) {  }

        Inst(): Inst("", nullptr, nullptr, 1) {  }
    
        // void operator()( cpu6502 &cpu )
        // {
        //     (cpu.*fA)();
        //     (cpu.*fE)();
        // }
    };

    Inst mCurrInstr;
    Inst mFtab[256];
    // -----------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------------

    uint8_t _N( uint16_t );
    uint8_t _NZ( uint16_t );
    uint8_t _NZC( uint16_t );
    uint8_t _NVZC( uint16_t x, uint8_t a , uint8_t b );

    void   _fetch();
    void   _decode();
    void   _execute();

    uint8_t  fetch08();
    uint16_t fetch16();
    // -----------------------------------------------------------------------------------------

    void     push08( uint8_t );
    void     push16( uint16_t );
    uint8_t  pop08();
    uint16_t pop16();

    void    _IntPush();
    void    _IntJump( uint16_t addr );
    void    _InstrNMI();
    void    _InstrIRQ();
    void    _InstrADC( uint8_t );

    int LoadACC();
    int LoadABS();
    int LoadABSX();
    int LoadABSY();
    int LoadIMM();
    int LoadIMP();
    int LoadIND();
    int LoadINDX();
    int LoadINDY();
    int LoadREL();
    int LoadZPG();
    int LoadZPGX();
    int LoadZPGY();

    void InstrUnimp();
    void InstrADC();
    void InstrAND();
    void InstrASL();
    void InstrBCC();
    void InstrBCS();
    void InstrBEQ();
    void InstrBIT();
    void InstrBMI();
    void InstrBNE();
    void InstrBPL();
    void InstrBRK();
    void InstrBVC();
    void InstrBVS();
    void InstrCLC();
    void InstrCLD();
    void InstrCLI();
    void InstrCLV();
    void InstrCMP();
    void InstrCPX();
    void InstrCPY();
    void InstrDEC();
    void InstrDEX();
    void InstrDEY();
    void InstrEOR();
    void InstrINC();
    void InstrINX();
    void InstrINY();
    void InstrJMP();
    void InstrJSR();
    void InstrLDA();
    void InstrLDX();
    void InstrLDY();
    void InstrLSR();
    void InstrNOP();
    void InstrORA();
    void InstrPHA();
    void InstrPHP();
    void InstrPLA();
    void InstrPLP();
    void InstrROL();
    void InstrROR();
    void InstrRTI();
    void InstrRTS();
    void InstrSBC();
    void InstrSEC();
    void InstrSED();
    void InstrSEI();
    void InstrSTA();
    void InstrSTX();
    void InstrSTY();
    void InstrTAX();
    void InstrTAY();
    void InstrTSX();
    void InstrTXA();
    void InstrTXS();
    void InstrTYA();
    void InstrWAI();

};
