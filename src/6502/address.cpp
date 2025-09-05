#include "6502.hpp"

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

uint8_t cpu6502::LoadACC( int cycle )
{
    mOpAC = true;
    return 0;
}

// OPC $LLHH - operand is address $HHLL *
uint8_t cpu6502::LoadABS( int cycle )
{
cycle_0:
    uint16_t lo = rdbus(PC++);

cycle_1:
    uint16_t hi = rdbus(PC++);

cycle_2:
    mOpAddr = (hi << 8) | lo;
    // mOpAddr = fetch16();
    return 0;
}

// OPC $LLHH,X - operand is address;
// effective address is address incremented by X with carry **
uint8_t cpu6502::LoadABSX( int cycle )
{
    mOpAddr = fetch16() + XR;
    return 0;
}

// OPC $LLHH,Y - operand is address;
// effective address is address incremented by Y with carry **
uint8_t cpu6502::LoadABSY( int cycle )
{
    mOpAddr = fetch16() + YR;
    return 0;
}


// OPC #$BB - operand is byte BB
uint8_t cpu6502::LoadIMM( int cycle )
{
    mOpAddr = PC++;
    return 0;
}


uint8_t cpu6502::LoadIMP( int cycle )
{
    return 0;
}


/**
 * OPC ($LLHH) operand is address;
 * effective address is contents of word at address: C.w($HHLL)
 */
uint8_t cpu6502::LoadIND( int cycle )
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
uint8_t cpu6502::LoadINDX( int cycle )
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
uint8_t cpu6502::LoadINDY( int cycle )
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
uint8_t cpu6502::LoadREL( int cycle )
{
    uint8_t byte = fetch08();
    mOpAddr = PC + *(int8_t*)(&byte);
    return 0;
}


/**
 * OPC $LL - operand is zeropage address (hi-byte is zero, address = $00LL)
 */
uint8_t cpu6502::LoadZPG( int cycle )
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
uint8_t cpu6502::LoadZPGX( int cycle )
{
    mOpAddr = ((uint16_t)fetch08() + XR) & 0x00FF;
    return 0;
}

/**
 * OPC $LL,Y - operand is zeropage address;
 * effective address is address incremented by Y without carry **
 */
uint8_t cpu6502::LoadZPGY( int cycle )
{
    mOpAddr = ((uint16_t)fetch08() + YR) & 0x00FF;
    return 0;
}

