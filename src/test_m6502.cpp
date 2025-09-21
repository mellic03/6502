#include <iostream>
#include <fstream>
#include <stdio.h>

#include <memu/configparser.hpp>
#include <memu/nes/nes.hpp>
#include <memu/hw/mostech/6502.hpp>

#include <json/json.hpp>
using json = nlohmann::json;



static void prepareTest( NesEmu::System *nes, auto data )
{
    syslog log("prepareTest %s", data["name"]);

    auto init = data["initial"];
    int pc = init["pc"];
    int s  = init["s"];
    int a  = init["a"];
    int x  = init["x"];
    int y  = init["y"];
    int p  = init["p"];

    log("pc: %d", pc);
    log("s:  %d", s);
    log("a:  %d", a);
    log("x:  %d", x);
    log("y:  %d", y);
    log("p:  %d", p);
}



int main( int argc, char **argv )
{
    int opts = argc - 2; // name, FILE

    memu::ConfigParser conf("boot.conf");
    for (int i=1; i<argc-1; i+=2)
        if (std::string(argv[i]) == "--conf")
            conf = memu::ConfigParser(argv[i+1]);
    conf.print();

    auto *nes = new NesEmu::System();
    nes->mPPU.loadPalette(conf["video"]["palette"]);


    auto root = json::parse(std::ifstream("test/00.json"));

    for (auto &data: root)
    {
        // std::string name = data["name"];
        // std::cout << "Name: " << name << "\n";
        prepareTest(nes, data);
    }

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

