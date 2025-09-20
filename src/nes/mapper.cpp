#include <memu/nes/mapper.hpp>
#include <memu/log.hpp>
#include "mapper/types.hpp"
#include <vector>
#include <cstdio>


NesEmu::Mapper *NesEmu::Mapper::MapGamePak( NesEmu::System &nes, GamePak *gpak )
{
    using ctor_t = Mapper* (*)(System&, GamePak*);

    #define X(Tp) [](System &n, GamePak *p) -> Mapper* { return new Tp(n, p); },
    static std::vector<ctor_t> ftab = { NESEMU_MAPPER_TYPES };
    #undef X

    auto  *fh = (iNES_File*)(gpak->data());
    size_t idx = (fh->MapperHi4 << 4) | fh->MapperLo4;

    // logasrt(idx < ftab.size(), "hi4=%u, lo4=%u", fh->MapperHi4, fh->MapperLo4);
    LogAsrt(idx < ftab.size(), "idx=%lu, ftab=fptr[%lu]\n", idx, ftab.size());

    return (ftab[idx])(nes, gpak);
}

