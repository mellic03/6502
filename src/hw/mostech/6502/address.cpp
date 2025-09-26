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


inline bool pageCrossed(uint16_t base, uint16_t addr)
{
    return (base & 0xFF00) != (addr & 0xFF00);
}


int m6502::LoadACC()
{
    mOpAC = true;
    return 0;
}

// OPC $LLHH - operand is address $HHLL *
int m6502::LoadABS()
{
    mOpAddr = fetch16();
    return 0;
}

// OPC $LLHH,X - operand is address;
// effective address is address incremented by X with carry **
int m6502::LoadABSX()
{
    mOpAddr = fetch16() + XR;
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

    uint16_t ind_lo  = fetch08();
    uint16_t ind_hi  = fetch08();
    uint16_t ptr_ind = (ind_hi << 8) | ind_lo;

    uint16_t abs_lo, abs_hi, ptr_abs;

    if (ind_lo == 0x00FF) {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind & 0xFF00); // emulate page-wrap bug
        mClock += 1;
    } else {
        abs_lo  = rdbus(ptr_ind + 0);
        abs_hi  = rdbus(ptr_ind + 1);
    }

    ptr_abs = (abs_hi << 8) | abs_lo;
    mOpAddr = ptr_abs;

    return 0;
}


/**
 * OPC ($LL,X) - operand is zeropage address;
 * effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
 */
int m6502::LoadINDX()
{
    uint8_t zp_addr = fetch08(); // operand
    uint8_t ptr_lo  = rdbus((zp_addr + XR) & 0xFF);
    uint8_t ptr_hi  = rdbus((zp_addr + XR + 1) & 0xFF);

    mOpAddr = (ptr_hi << 8) | ptr_lo;
    return 0;
}

/**
 * OPC ($LL),Y - operand is zeropage address;
 * effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
 */
int m6502::LoadINDY()
{
    uint8_t zp_addr = fetch08();
    uint8_t ptr_lo  = rdbus(zp_addr & 0xFF);
    uint8_t ptr_hi  = rdbus((zp_addr + 1) & 0xFF);

    uint16_t base   = (ptr_hi << 8) | ptr_lo;
    mOpAddr         = base + YR;

    return 0;
}

/**
 * OPC $BB - branch target is PC + signed offset BB ***
 */
int m6502::LoadREL()
{
    mOpOfst = (int8_t)fetch08();
    mOpAddr = PC + mOpOfst;

    // uint8_t byte = fetch08();
    // mOpAddr = PC + *(int8_t*)(&byte);

    return ((mOpAddr>>8) == (PC>>8)) ? 0 : 1;
}

/**
 * OPC $LL - operand is zeropage address (hi-byte is zero, address = $00LL)
 */
int m6502::LoadZPG()
{
    mOpAddr = fetch08();
    return 0;
}

/**
 * OPC $LL,X - operand is zeropage address;
 * effective address is address incremented by X without carry **
 */
int m6502::LoadZPGX()
{
    mOpAddr = (fetch08() + XR) & 0xFF;
    return 0;
}

/**
 * OPC $LL,Y - operand is zeropage address;
 * effective address is address incremented by Y without carry **
 */
int m6502::LoadZPGY()
{
    mOpAddr = (fetch08() + YR) & 0xFF;
    return 0;
}

