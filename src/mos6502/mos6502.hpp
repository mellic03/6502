#pragma once
#include "../hw/hwmodule.hpp"


class MOS6502: public Emu::HwModule
{
public:
    MOS6502( Emu::AddrSpace& );
    virtual void tick() override;
    virtual void reset() override;

    void sig_irq(); // { m_pins.irq = 0; }
    void sig_res(); // { m_pins.res = 0; }
    void sig_nmi(); // { m_pins.nmi = !m_pins_prev.nmi; }

private:
    uint8_t  mInvalidOp;
    uint8_t  mCurrOp;
    size_t   mCycles;
    size_t   mOpCount;
    bool     mOpAC;
    uint16_t mOpAddr;

    uint8_t  AC, XR, YR, SP;

    union {
        uint16_t PC;
        struct {
            uint8_t PC_lo;
            uint8_t PC_hi;
        } __attribute__((packed));
    };

    union {
        ubyte byte;
        struct {
            ubyte C :1;
            ubyte Z :1;
            ubyte I :1;
            ubyte D :1;
            ubyte U :1;
            ubyte B :1;
            ubyte V :1;
            ubyte N :1;
        } __attribute__((packed));
    } SSR = { 0b00100100 };

    union {
        uint8_t byte;
        struct {
            uint8_t irq :1;
            uint8_t res :1;
            uint8_t nmi :1;
            uint8_t wai :1;
        };
    } mPinsCurr={0b0011}, mPinsPrev={0b0011};

    // Inst mCurrInstr;
    // Inst mFtab[256];

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

    void _IntPush();
    void _IntJump(uword);
    void _InstrNMI();
    void _InstrIRQ();
    void _InstrRES();
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
    void InstrWAI();

};
