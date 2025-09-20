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
    std::vector<char> mBuf;

    using KeyVal = std::map<std::string, std::string>;
    std::map<std::string, KeyVal> mConf;

    std::string _readSection();
    std::string _readLabel();
    std::string _readTo(char);
    std::string _readString();
    std::string _readValue();

    void skip(char);
    char peek();
    char advance();
    char retreat();
    char match(char);

public:
    ConfigParser(const char *filepath);
    void print();
    KeyVal &operator[]( const std::string &S ) { return mConf[S]; }
};

