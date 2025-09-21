#include <iostream>
#include <fstream>
#include <stdio.h>

#include <memu/configparser.hpp>
#include <memu/nes/nes.hpp>
#include <memu/hw/mostech/6502.hpp>

#include <json/json.hpp>
using json = nlohmann::json;


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


    std::ifstream stream("test/00.json");
    json jsn;
    stream >> jsn;

    auto init = jsn[1];

    for (auto &pair: init["ram"])
    {
        int addr = pair[0];  // not pair["0"]
        int val  = pair[1];
        std::cout << "RAM[" << addr << "] = " << val << "\n";
    }

    // std::cout << "jsn[\"name\"]: " << jsn["name"] << "\n";

    // for (auto &node: data)
    // {
    //     data.
    //     node.
    // }

    return 0;
}

