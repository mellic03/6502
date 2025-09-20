#include <memu/hw/mostech/6502.hpp>


uint8_t m6502::fetch08()
{
    return rdbus(PC++);
}

uint16_t m6502::fetch16()
{
    uint8_t lo = rdbus(PC++);
    uint8_t hi = rdbus(PC++);
    return ((uint16_t)hi << 8) | (uint16_t)lo;
}


int m6502::LoadACC()
{
    mOpAC = true;
    return 0;
}

// OPC $LLHH - operand is address $HHLL *
int m6502::LoadABS()
{
    uint16_t lo = rdbus(PC++);
    uint16_t hi = rdbus(PC++);
    mOpAddr = (hi << 8) | lo;
    return 0;
}

// OPC $LLHH,X - operand is address;
// effective address is address incremented by X with carry **
int m6502::LoadABSX()
{
    uint16_t lo = rdbus(PC++);
    uint16_t hi = rdbus(PC++);
    mOpAddr = ((hi << 8) | lo) + XR;
    return 0;
}

// OPC $LLHH,Y - operand is address;
// effective address is address incremented by Y with carry **
int m6502::LoadABSY()
{
    mOpAddr = fetch16() + YR;
    return 0;
}

// OPC #$BB - operand is byte BB
int m6502::LoadIMM()
{
    mOpAddr = PC++;
    return 0;
}

int m6502::LoadIMP()
{
    return 0;
}

/**
 * OPC ($LLHH) - operand is address;
 * effective address is contents of word at address: C.w($HHLL)
 */
int m6502::LoadIND()
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
int m6502::LoadINDX()
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
int m6502::LoadINDY()
{
    uint16_t lo = rdbus(PC++);
    uint16_t hi = rdbus(PC++);
    uint16_t p  = (hi << 8) | lo;

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
int m6502::LoadREL()
{
    uint8_t byte = fetch08();
    mOpAddr = PC + *(int8_t*)(&byte);
    return ((mOpAddr>>8) == (PC>>8)) ? 0 : 1;
}

/**
 * OPC $LL - operand is zeropage address (hi-byte is zero, address = $00LL)
 */
int m6502::LoadZPG()
{
    mOpAddr = rdbus(PC++);
    return 0;
}

/**
 * OPC $LL,X - operand is zeropage address;
 * effective address is address incremented by X without carry **
 */
int m6502::LoadZPGX()
{
    mOpAddr = rdbus(PC++) + XR;
    return 0;
}

/**
 * OPC $LL,Y - operand is zeropage address;
 * effective address is address incremented by Y without carry **
 */
int m6502::LoadZPGY()
{
    mOpAddr = rdbus(PC++) + YR;
    return 0;
}

