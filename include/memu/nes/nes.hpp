#pragma once

#include <memu/addrspace.hpp>
#include <memu/configparser.hpp>
#include <memu/display.hpp>
#include "cpu.hpp"
#include "ppu.hpp"

namespace NesEmu
{
    extern memu::ConfigParser CONF;
    class GamePak;
    class System;
}


/*
    https://www.emulationonline.com/systems/nes/controller/

    0x4016 (write) : Controller Strobe
    ----------------------------------------------------------------------------
    The strobe is controlled via a MMIO register at 0x4016.
    Only one bit is used, and it is the 1s place or least significant bit.
    So writing 0x01 to 0x4016 sets the strobe, and writing 0x00 releases it.
    After a set and release the shift register is loaded with the state of all 8 buttons.

    0x4016 and 0x4017 (read) : Controller state
    ----------------------------------------------------------------------------
    Finally controller bits can be read out one at a time.
    These are communicated via bit 1 of the value read from this address.
    Player 1 is read from 0x4016, and player 2 from 0x4017.
    Though each controller is read from a separate address, both controllers
    share the strobe write at 0x4016.

    | Line |  Used by                                               |
    |---------------------------------------------------------------|
    | D0   | NES standard controller, Famicom hardwired controller. |
    | D1   | Famicom expansion port controller.                     |
    | D2   | Famicom microphone (controller 2 only).                |
    | D3   | Zapper light sense.                                    |
    | D4   | Zapper trigger.                                        |
    |---------------------------------------------------------------|

    0 - A
    1 - B
    2 - Select
    3 - Start
    4 - Up
    5 - Down
    6 - Left
    7 - Right
*/

class NesEmu::System
{
private:
    size_t mClocks = 0;

    struct {
        ubyte ioNmi;
        ubyte ioRES;
        ubyte ioIRQ;
        ubyte ioCLK;
    };

public:
    memu::AddrSpace mBusCPU;
    memu::AddrSpace mBusPPU;
    NesCPU     mCPU;
    NesPPU     mPPU;
    GamePak   *mGPak;
    EmuWindow *mWin;

    System();
    System(const System&) = delete;
    System &operator=(const System&) = delete;
    void loadGamePak(GamePak*);
    void tick();
    void reset() { ioRES=0; }
    // StdCtl &getStdCtl0() { return mStdCtl0; }
    // StdCtl &getStdCtl1() { return mStdCtl1; }
};

