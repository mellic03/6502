#include "6502.hpp"

#include <algorithm>
template <typename T>
T clamp( T x, T a, T b ) { return std::max(a, std::min(x, b)); }

#define DISPATCH_01() \
    void *Jtab[] = { &&cycle_0, &&cycle_1 }; \
    goto *Jtab[clamp(cycle, 0, 1)]

#define DISPATCH_012() \
    void *Jtab[] = { &&cycle_0, &&cycle_1, &&cycle_2 }; \
    goto *Jtab[clamp(cycle, 0, 2)]

#define DISPATCH_0123() \
    void *Jtab[] = { &&cycle_0, &&cycle_1, &&cycle_2, &&cycle_3 }; \
    goto *Jtab[clamp(cycle, 0, 3)]


uint8_t cpu6502::fetch08()
{
    return rdbus(PC++);
}

uint16_t cpu6502::fetch16()
{
    uint8_t lo = rdbus(PC++);
    uint8_t hi = rdbus(PC++);
    return ((uint16_t)hi << 8) | (uint16_t)lo;
}

int cpu6502::LoadACC()
{
    mOpAC = true;
    return 0;
}


// OPC $LLHH - operand is address $HHLL *
int cpu6502::LoadABS()
{
    uint16_t lo = rdbus(PC++);
    uint16_t hi = rdbus(PC++);
    mOpAddr = (hi << 8) | lo;

    return 0;
//     static uint16_t lo, hi;
//     DISPATCH_012();

// cycle_0:
//     lo = rdbus(PC++);
//     return cycle+1;

// cycle_1:
//     hi = rdbus(PC++);
//     return cycle+1;

// cycle_2:
//     mOpAddr = (hi << 8) | lo;
    // return -1;
}

// OPC $LLHH,X - operand is address;
// effective address is address incremented by X with carry **
int cpu6502::LoadABSX()
{
    uint16_t lo = rdbus(PC++);
    uint16_t hi = rdbus(PC++);
    mOpAddr = ((hi << 8) | lo) + XR;
    mOpData = rdbus(mOpAddr);
    return 0;

//    static uint16_t lo, hi;
//     DISPATCH_012();

// cycle_0:
//     lo = rdbus(PC++);
//     return cycle+1;

// cycle_1:
//     hi = rdbus(PC++);
//     return cycle+1;

// cycle_2:
//     mOpAddr = ((hi << 8) | lo) + XR;
//     mOpData = rdbus(mOpAddr);
    // return -1;
}

// OPC $LLHH,Y - operand is address;
// effective address is address incremented by Y with carry **
int cpu6502::LoadABSY()
{
    mOpAddr = fetch16() + YR;
    mOpData = rdbus(mOpAddr);
    return 0;
}


// OPC #$BB - operand is byte BB
int cpu6502::LoadIMM()
{
cycle_0:
    mOpAddr = PC++;
    mOpData = rdbus(mOpAddr);
    return 0;
}


int cpu6502::LoadIMP()
{
    return 0;
}


/**
 * OPC ($LLHH) operand is address;
 * effective address is contents of word at address: C.w($HHLL)
 */
int cpu6502::LoadIND()
{
    uint16_t ind_lo  = fetch08() & 0x00FF;
    uint16_t ind_hi  = fetch08() & 0x00FF;
    uint16_t ptr_ind = (ind_hi << 8) | ind_lo;

    uint16_t abs_lo  = 0x0000;
    uint16_t abs_hi  = 0x0000;
    uint16_t ptr_abs = 0x0000;

    if (ind_lo == 0x00FF)
    {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind + 1) & 0xFF00;
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    else
    {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind + 1);
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    mOpAddr = ptr_abs;

    return 0;
}

/**
 * OPC ($LL,X) - operand is zeropage address;
 * effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
 */
int cpu6502::LoadINDX()
{
    uint16_t ind_lo  = fetch08() & 0x00FF;
    uint16_t ind_hi  = fetch08() & 0x00FF;
    uint16_t ptr_ind = (ind_hi << 8) | ind_lo;

    uint16_t abs_lo  = 0x0000;
    uint16_t abs_hi  = 0x0000;
    uint16_t ptr_abs = 0x0000;

    if (ind_lo == 0x00FF)
    {
        abs_lo  = rdbus(ptr_ind + XR + 0);
        abs_hi  = rdbus(ptr_ind + XR + 1) & 0xFF00;
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    else
    {
        abs_lo  = rdbus(ptr_ind + XR + 0);
        abs_hi  = rdbus(ptr_ind + XR + 1);
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    mOpAddr = ptr_abs;

    return 0;
}

/**
 * OPC ($LL),Y - operand is zeropage address;
 * effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
 */
int cpu6502::LoadINDY()
{
    uint16_t ind_lo  = fetch08() & 0x00FF;
    uint16_t ind_hi  = fetch08() & 0x00FF;
    uint16_t ptr_ind = (ind_hi << 8) | ind_lo;

    uint16_t abs_lo  = 0x0000;
    uint16_t abs_hi  = 0x0000;
    uint16_t ptr_abs = 0x0000;

    if (ind_lo == 0x00FF)
    {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind + 1) & 0xFF00;
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    else
    {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind + 1);
        ptr_abs = (abs_hi << 8) | abs_lo;
    }

    mOpAddr = ptr_abs + YR;

    return 0;
}

/**
 * OPC $BB - branch target is PC + signed offset BB ***
 */
int cpu6502::LoadREL()
{
    uint8_t byte = fetch08();
    mOpAddr = PC + *(int8_t*)(&byte);

    if (mOpAddr/256 == PC/256)
    {
        return 0;
    }

    else
    {
        return 1;
    }
}


/**
 * OPC $LL - operand is zeropage address (hi-byte is zero, address = $00LL)
 */
int cpu6502::LoadZPG()
{
    mOpAddr = (uint16_t)fetch08() & 0x00FF;
    return 0;
    // uint16_t addr = mBus[PC++] & 0x00FF;
    // return &mBus[addr];
}

/**
 * OPC $LL,X - operand is zeropage address;
 * effective address is address incremented by X without carry **
 */
int cpu6502::LoadZPGX()
{
    mOpAddr = ((uint16_t)fetch08() + XR) & 0x00FF;
    return 0;
}

/**
 * OPC $LL,Y - operand is zeropage address;
 * effective address is address incremented by Y without carry **
 */
int cpu6502::LoadZPGY()
{
    mOpAddr = ((uint16_t)fetch08() + YR) & 0x00FF;
    return 0;
}

