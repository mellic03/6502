#include <memu/configparser.hpp>
#include <memu/file.hpp>
#include <memu/log.hpp>
#include <fstream>

using namespace memu;


static int iidx;
static int lline;
static int ccol;


void ConfigParser::skip( char )
{
    char ch = mBuf[iidx];

    while (iidx < mBuf.size() && ch && (ch==' '))
    {
        ch = mBuf[iidx++];
    }

    if (iidx >= mBuf.size() - 1)
    {
        iidx = mBuf.size() - 1;
    }
}


char ConfigParser::peek()
{
    skip(' ');

    if (iidx >= mBuf.size() - 1)
    {
        iidx = mBuf.size() - 1;
        return '\0';
    }
    return mBuf[iidx];
}


char ConfigParser::advance()
{
    char ch = peek();
    if (!ch) return ch;

    iidx += 1;
    ccol += 1;

    if (ch == '\n')
    {
        lline += 1;
        ccol = 1;
    }

    return ch;
}


char ConfigParser::retreat()
{
    iidx -= 1;
    return peek();
}


char ConfigParser::match( char ch )
{
    if (peek() == ch)
        return advance();
    return '\0';    
}



std::string ConfigParser::_readLabel()
{
    std::string str = "";

    while (char ch = advance())
    {
        if (ch=='=')
            break;
        if (!isalnum(ch))
            break;
        str.push_back(ch);
    }

    return str;
}


std::string ConfigParser::_readTo( char stop )
{
    std::string str = "";
    while (char ch = advance())
    {
        if (ch==stop) break;
        str.push_back(ch);
    }
    return str;
}


ConfigParser::ConfigParser( const char *path )
:   mPath(path), mIdx(0)
{
    for (char ch: loadFileRaw(path))
    {
        if (ch != ' ')
        {
            mBuf.push_back(ch);
        }
    }

    iidx  = 0;
    lline = 1;
    ccol  = 1;

    std::string section = "GLOBAL";

    while (char ch = peek())
    {
        if (ch=='\n')
        {
            advance();
        }

        else if (match('['))
        {
            section = _readTo(']');
        }
    
        else if (isalpha(ch))
        {
            std::string key = _readLabel();

            if (match('\"'))
                mConf[section][key] = _readTo('\"');
            else
                mConf[section][key] = _readTo('\n');
        }
    }
}



void ConfigParser::print()
{
    for (auto &[section, keyval]: mConf)
    {
        syslog log("Section %s", section.c_str());

        for (auto &[key, value]: keyval)
        {
            log("%s = \"%s\"", key.c_str(), value.c_str());
        }
    }
}
