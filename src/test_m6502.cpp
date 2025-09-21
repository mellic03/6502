#include <iostream>
#include <fstream>
#include <stdio.h>

#include <memu/addrspace.hpp>
#include <memu/configparser.hpp>
#include <memu/nes/nes.hpp>
#include <memu/nes/gamepak.hpp>
#include <memu/hw/mostech/6502.hpp>

#include <json/json.hpp>
using json = nlohmann::json;


static void performTest( m6502 &cpu, auto data )
{
    auto init = data["initial"];
    uword iPC  = init["pc"];
    ubyte iSP  = init["s"];
    ubyte iAC  = init["a"];
    ubyte iXR  = init["x"];
    ubyte iYR  = init["y"];
    ubyte iSSR = init["p"];

    auto fnal = data["final"];
    uword fPC  = fnal["pc"];
    ubyte fSP  = fnal["s"];
    ubyte fAC  = fnal["a"];
    ubyte fXR  = fnal["x"];
    ubyte fYR  = fnal["y"];
    ubyte fSSR = fnal["p"];

    printf("PC:  0x%04X -> 0x%04X\n", iPC, fPC);
    printf("SP:  0x%02X   -> 0x%02X\n", iSP, fSP);
    printf("AC:  0x%02X   -> 0x%02X\n", iAC, fAC);
    printf("XR:  0x%02X   -> 0x%02X\n", iXR, fXR);
    printf("YR:  0x%02X   -> 0x%02X\n", iYR, fYR);
    printf("SSR: 0x%02X   -> 0x%02X\n", iSSR, fSSR);
    printf("\n");

    for (auto &pair: init["ram"])
    {
        uword addr = pair[0];
        ubyte val  = pair[1];
        cpu.mBus.write(addr, val);
        printf("mem[0x%04X] = 0x%02X\n", addr, val);
    }
    printf("\n");


    cpu.PC = iPC;
    cpu.SP = iSP;
    cpu.AC = iAC;
    cpu.XR = iXR;
    cpu.YR = iYR;
    cpu.SSR.as_byte = iSSR;

    // while (cpu.PC != fPC)
    for (int i=0; i<4; i++)
    {
        cpu.tick();
    }

    printf("\n");
    printf("PC:  0x%04X\n", cpu.PC);
    printf("SP:  0x%02X\n", cpu.SP);
    printf("AC:  0x%02X\n", cpu.AC);
    printf("XR:  0x%02X\n", cpu.XR);
    printf("YR:  0x%02X\n", cpu.YR);
    printf("SSR: 0x%02X\n", cpu.SSR.as_byte);
    printf("\n");
}



int main( int argc, char **argv )
{
    int opts = argc - 2; // name, FILE

    memu::ConfigParser conf("boot.conf");
    for (int i=1; i<argc-1; i+=2)
        if (std::string(argv[i]) == "--conf")
            conf = memu::ConfigParser(argv[i+1]);

    auto *nes = new NesEmu::System();
    nes->loadGamePak(new NesEmu::GamePak(conf["boot"]["rom"]));
    nes->mPPU.loadPalette(conf["video"]["palette"]);

    // nes->mBusCPU.mapRange(0x0000, 0x7FFF, memu::RWX_RW, new ubyte[32*1024], 32*1024);

    // auto root = json::parse(std::ifstream("test/00.json"));
    // for (auto &data: root)
    // {
    //     performTest(nes, data);
    // }

    auto data = json::parse(std::ifstream("test/02.json"));
    performTest(nes->mCPU, data);

    // // Access "initial" state
    // auto init = data["initial"];
    // int pc_init = init["pc"];
    // int s_init  = init["s"];
    // int a_init  = init["a"];
    // int x_init  = init["x"];
    // int y_init  = init["y"];
    // int p_init  = init["p"];

    // std::cout << "Initial PC: " << pc_init << "\n";
    // std::cout << "Initial s:  " << s_init << "\n";
    // std::cout << "Initial a:  " << a_init << "\n";
    // std::cout << "Initial x:  " << x_init << "\n";
    // std::cout << "Initial y:  " << y_init << "\n";
    // std::cout << "Initial p:  " << p_init << "\n";

    // // Access RAM array inside "initial"
    // for (auto& pair : init["ram"]) {
    //     int addr = pair[0];
    //     int val  = pair[1];
    //     std::cout << "RAM[" << addr << "] = " << val << "\n";
    // }

    // // Access "final" state similarly
    // auto fin = data["final"];
    // int pc_final = fin["pc"];
    // std::cout << "Final PC: " << pc_final << "\n";

    // // Access "cycles"
    // for (auto& cycle : data["cycles"]) {
    //     int addr     = cycle[0];
    //     int value    = cycle[1];
    //     std::string op = cycle[2];
    //     std::cout << "Cycle: addr=" << addr << " val=" << value << " op=" << op << "\n";
    // }

    return 0;
}

