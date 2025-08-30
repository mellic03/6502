#include <stdio.h>
#include <assert.h>

#include "./hw/ppc.hpp"


int main( int argc, char **argv )
{
    ppc_board_t main_board;
    // addr_device bus(emu::BUSW_32);

    // emu::mono_t nbytes = emu::kilo_t(512);
    // printf("nbytes: %lu\n", nbytes.value);

    for (int i=0; i<8; i++)
    {
        main_board.mMainBus->broadcast((uint8_t)i);
    }

    return 0;
}

