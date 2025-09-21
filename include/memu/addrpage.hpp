#pragma once

#include "types.hpp"
#include "rwx.hpp"
#include "hwmodule.hpp"


namespace memu
{
    class PageEntry;
    class iPageHandler;
}


class memu::PageEntry
{
public:
    static ubyte *pgdummy;
    ubyte        *mBuf;
    ubyte         mMask;
    iPageHandler *mHandler;

    PageEntry(): PageEntry(pgdummy, 0xFF) {  }
    PageEntry(ubyte *buf, ubyte mask): mBuf(buf), mMask(mask), mHandler(nullptr) {  }
    PageEntry(iPageHandler *h): mBuf(pgdummy), mMask(0xFF), mHandler(h) { }
};


class memu::iPageHandler
{
public:
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

