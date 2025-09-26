#pragma once

#include "types.hpp"
#include "rwx.hpp"
#include "hwmodule.hpp"

namespace memu
{
    class iPageHandler;
}

class memu::iPageHandler
{
public:
    virtual ubyte read(addr_t) { return 0; };
    virtual void write(addr_t, ubyte) { return; };

};


// class memu::iDevicePage: public iPageEntry
// {
// private:
//     // HwModule *mDev;

// public:
//     iDevicePage(ubyte *buf, RWX_ rwx)
//     :   iPageEntry(buf, rwx) {  }

//     virtual ubyte read(addr_t) = 0;
//     virtual void write(addr_t, ubyte) = 0;
// };

