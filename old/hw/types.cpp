#include "types.hpp"

emu::mono_t::mono_t( const mono_t &B ): value(B.value) {  }
emu::mono_t::mono_t( const kilo_t &K ): value(KILO*K.value) {  }
emu::mono_t::mono_t( const mega_t &M ): value(MEGA*M.value) {  }

emu::kilo_t::kilo_t( const mono_t &B ): value(B.value / KILO) {  }
emu::kilo_t::kilo_t( const kilo_t &K ): value(K.value) {  }
emu::kilo_t::kilo_t( const mega_t &M ): value(KILO * M.value) {  }

emu::mega_t::mega_t( const mono_t &B ): value(B.value / MEGA) {  }
emu::mega_t::mega_t( const kilo_t &K ): value(K.value / KILO) {  }
emu::mega_t::mega_t( const mega_t &M ): value(M.value) {  }
