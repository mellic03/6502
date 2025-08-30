#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>


int main( int argc, char **argv )
{
    static uint8_t prog[512];
    prog[0] = 0x01;
    prog[1] = 0x06;
    prog[2] = 0x07;
    prog[3] = 0x08;

    // ((uint32_t*)prog)[0] = ((uint32_t)0x01 << 24) + ((uint32_t)0x06 << 16) + ((uint32_t)0x07 << 8) + 0x08;
    // ((uint32_t*)prog)[0] = 0x01060708;
    // prog[0] = 0x/uint16_t)0x02 << 8) + 0x01;

    printf("0x%08x\n", ((uint32_t*)prog)[0]);
    // prog[1] = 0x02;
    // prog[2] = 0x03;
    // prog[3] = 0x04;
    // prog[4] = 0x00;
    // prog[5] = 0x00;

    std::ofstream stream("rom.bin", std::ios::binary);
    stream.write((const char*)prog, 32);
    stream.close();

    return 0;
}
