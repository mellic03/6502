#ifndef EMU_TYPES_H
#define EMU_TYPES_H

#include <cstddef>
#include <cstdint>


namespace emu
{
    static constexpr size_t KILO = 1024;
    static constexpr size_t MEGA = 1024*KILO;
    static constexpr size_t GIGA = 1024*MEGA;

    enum BUSW_: size_t
    {
        BUSW_08,
        BUSW_16,
        BUSW_32,
        BUSW_64,
        BUSW_128,
    };

    struct mono_t;
    struct kilo_t;
    struct mega_t;

    struct mono_t
    {
        size_t value;
        mono_t( size_t n=0 ): value(n) {  };
        mono_t( const mono_t &B );
        mono_t( const kilo_t &K );
        mono_t( const mega_t &M );
    };

    struct kilo_t
    {
        size_t value;
        kilo_t( size_t n=0 ): value(n) {  };
        kilo_t( const mono_t &B );
        kilo_t( const kilo_t &K );
        kilo_t( const mega_t &M );
    };

    struct mega_t
    {
        size_t value;
        mega_t( size_t n=0 ): value(n) {  };
        mega_t( const mono_t &B );
        mega_t( const kilo_t &K );
        mega_t( const mega_t &M );
    };

}


#endif
