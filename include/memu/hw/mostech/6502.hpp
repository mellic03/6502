#pragma once

#include <memu/hwmodule.hpp>
#include <memu/memory.hpp>
#include "6502_detail.hpp"


class m6502: public memu::HwModule, public m6502_detail::BaseHw
{
public:
    typedef int (m6502::*AddrFn)();
    typedef void (m6502::*ExecFn)();

    Memory2kRW mRAM;
    bool       mWaiting;
    uint8_t    mInvalidOp;
    uint8_t    mCurrOp;
    size_t     mOpCount;
    bool       mOpAC;
    uint16_t   mOpAddr;

    // uint16_t   mScanLine = 0;
    // uint16_t   mScanDot = 0;

    m6502( memu::AddrSpace& );
    virtual void tick() override;
    virtual void reset() override;
    void wait() { mWaiting = true; }


private:
    struct Inst;
    m6502::Inst *mCurrInstr;
    m6502::Inst *mFtab;

    uint8_t _N(uint8_t);
    uint8_t _Z(uint8_t);
    uint8_t _NZ(uint8_t);
    uint8_t _NZC(uint8_t);
    uint8_t _NVZC(uint8_t x, uint8_t a, uint8_t b);

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

    int  LoadACC();
    int  LoadABS();
    int  LoadABSX();
    int  LoadABSY();
    int  LoadIMM();
    int  LoadIMP();
    int  LoadIND();
    int  LoadINDX();
    int  LoadINDY();
    int  LoadREL();
    int  LoadZPG();
    int  LoadZPGX();
    int  LoadZPGY();

    void _NMI();
    void _RES();
    void _IRQ();
    void _BRK();

    void _IntPush(ubyte B, ubyte U=0);
    void _IntJump(uword);
    void _InstrADC(ubyte);


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

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

};


struct m6502::Inst
{
    const char *label;
    ExecFn fE;
    AddrFn fA;
    int cycles;
    
    Inst(): Inst("", nullptr, nullptr, 1) {  }
    Inst( const char *str, ExecFn fe, AddrFn fa, int cyc )
    : label(str), fE(fe), fA(fa), cycles(cyc) {  }
};
