#pragma once

#include "types.hpp"
#include "rwx.hpp"
#include "hwmodule.hpp"


namespace memu
{
    class iPageEntry;
    class iDevicePage;

    class PageEntry;
}


class memu::iPageEntry
{
public:
    virtual ubyte read(addr_t) = 0;
    virtual void write(addr_t, ubyte) = 0;

};


class memu::iDevicePage: public iPageEntry
{
private:
    HwModule *mDev;

public:
    iDevicePage(HwModule *dev)
    :   mDev(dev) {  };

    virtual ubyte read(addr_t) = 0;
    virtual void write(addr_t, ubyte) = 0;
};



class memu::PageEntry: public iPageEntry
{
private:
    ubyte *mBuf;
    RWX_   mRWX;

public:
    PageEntry(ubyte *buf, RWX_ rwx)
    :   mBuf(buf), mRWX(rwx) {  };

    virtual ubyte read(addr_t) override;
    virtual void write(addr_t, ubyte) override;

};


