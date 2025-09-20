#pragma once
#include <map>
#include <string>
#include <vector>

namespace memu
{
    class ConfigParser;
}


class memu::ConfigParser
{
private:
    const char       *mPath;
    int               mIdx;
    // std::vector<char> mBuf;
    std::map<std::string, std::string> mConf;

    std::string _readSection();
    std::string _readLabel();
    std::string _readValue();
    std::string _readLabelValue();

    char peek();
    char advance();
    char check(char);

public:
    ConfigParser(const char *filepath);
};

