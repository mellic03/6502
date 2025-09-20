#include <memu/hw/ricoh/2A03.hpp>
#include <memu/addrspace.hpp>

using namespace memu;


Ricoh2A03::Ricoh2A03( AddrSpace &bus )
:   m6502(bus)
{

}

