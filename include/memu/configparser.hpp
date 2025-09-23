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
    using BiMap  = std::map<std::string, std::string>;
    using TriMap = std::map<std::string, BiMap>;
    const char       *mPath;
    int               mIdx;
    std::vector<char> mBuf;
    TriMap            mData;

    class nomap_t
    {
    private:
        bool mExists;
        std::string mValue;

    public:
        nomap_t(bool e, const std::string &v): mExists(e), mValue(v) {  }
        operator bool() const { return mExists; }
        operator std::string() const { return mValue; }
        operator const char*() const { return mValue.c_str(); }
    };


    class bimap_t
    {
    private:
        bool mExists;
        std::map<std::string, std::string> &mData;

    public:
        bimap_t(bool e, BiMap &d): mExists(e), mData(d) {  }
        operator bool() const { return mExists; }

        nomap_t operator[]( const char *key )
        {
            return (*this)[std::string(key)];
        }

        nomap_t operator[]( const std::string &key )
        {
            if (mData.contains(key))
                return nomap_t(true, mData[key]);
            return nomap_t(false, "");
        }

    };

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
    ConfigParser( const char *path );
    void print();

    bimap_t operator[]( const std::string &sect )
    {
        if (mData.contains(sect))
            return bimap_t(true, mData[sect]);
        return bimap_t(false, mData["__global__"]);
    }
};

