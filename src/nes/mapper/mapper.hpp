#pragma once

#include "../gamepak.hpp"
#include "../system.hpp"

namespace NesEmu
{
    void ExecuteMapper( int mapno, System& );
}

void Mapper00_NROM(NesEmu::System&);
void Mapper01_MMC1(NesEmu::System&);
void Mapper02_UxROM(NesEmu::System&);
void Mapper03_CNROM(NesEmu::System&);
void Mapper04_MMC3(NesEmu::System&);


struct HwMapper
{
    struct pml0_t
    {
        uint16_t base;
        uint16_t mask;
        uint8_t  rwx;
    };

    struct pml1_t
    {
        pml0_t *pml0;
        uint16_t base;
        uint16_t mask;
        uint8_t  rwx;
    };

    struct pml2_t
    {
        pml1_t *pml1;
        uint16_t base;
        uint16_t mask;
        uint8_t  rwx;
    };

    struct pml3_t
    {
        pml2_t  *pml2;
        uint16_t base;
        uint16_t mask;
        uint8_t  rwx;
    };

    pml3_t mPML3[16];



    ubyte read( uint16_t addr )
    {
        union {
            uint16_t word;
            struct {
                uint8_t nib0 :4;
                uint8_t nib1 :4;
                uint8_t nib2 :4;
                uint8_t nib3 :4;
            };
        } U = { addr };

        int idx3 = U.nib3;
        int idx2 = U.nib2;
        int idx1 = U.nib1;
        int idx0 = U.nib0;

        pml3_t &pml3 = mPML3[idx3];
        pml2_t &pml2 = pml3.pml2[idx2];
        pml1_t &pml1 = pml2.pml1[idx1];
        pml0_t &pml0 = pml1.pml0[idx0];
        pml0.base
    }

};



