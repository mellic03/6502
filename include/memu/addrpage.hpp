#pragma once

#include "types.hpp"
#include "rwx.hpp"
#include "hwmodule.hpp"


namespace memu
{
    class iPageEntry;
    class PageEntry;
}


class memu::iPageEntry
{
protected:
    ubyte *mBuf;
    RWX_   mRWX;

public:
    static ubyte *pgdummy;

    iPageEntry(ubyte *buf, RWX_ rwx): mBuf(buf), mRWX(rwx) {  }
    iPageEntry(): iPageEntry(pgdummy, RWX_RW) {  }

    virtual ubyte read(addr_t) = 0;
    virtual void write(addr_t, ubyte) = 0;

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



class memu::PageEntry: public iPageEntry
{
public:
    PageEntry(ubyte *buf, RWX_ rwx)
    :   iPageEntry(buf, rwx) {  }

    virtual ubyte read(addr_t) override;
    virtual void write(addr_t, ubyte) override;

};


