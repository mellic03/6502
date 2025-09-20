#include <memu/configparser.hpp>
#include <memu/log.hpp>
#include <fstream>

using namespace memu;


static const char *skipch( const char *str, char ch )
{
    while (*str == ch)
        str++;
    return str;
}


static std::vector<char> bbuf;
static int iidx;
static int lline;
static int ccol;

char ConfigParser::peek()
{
    if (iidx >= bbuf.size() - 1)
    {
        iidx = bbuf.size() - 1;
        return '\0';
    }
    return bbuf[iidx];
}

char ConfigParser::advance()
{
    if (iidx >= bbuf.size() - 1)
    {
        iidx = bbuf.size() - 1;
        ccol += 1;
        return '\0';
    }
    return bbuf[iidx++];
}

char ConfigParser::check( char ch )
{
    if (peek() == ch)
        return advance();
    return '\0';    
}


std::string ConfigParser::_readSection()
{
    std::string str = "";

    advance();
    while (char ch = advance())
    {
        if (ch==']')
            break;
        str.push_back(ch);
    }
    printf("[_readSection] str=\"%s\"\n", str.c_str());

    while (check(' ')) {  }

    return str;
}


std::string ConfigParser::_readLabel()
{
    std::string str = "";

    while (char ch = advance())
    {
        if (!isalpha(ch))
            break;
        str.push_back(ch);
    }
    printf("[_readLabel] str=\"%s\"\n", str.c_str());

    while (check(' ')) {  }
    LogAsrt(
        peek()=='=',
        "Invalid syntax, expected \'=\', received \'%c\'. (line %d col %d, \"%s\")\n",
        peek(), lline, ccol, mPath
    );

    return str;
}

std::string ConfigParser::_readLabelValue()
{
    std::string label = _readLabel();

    while (char ch = advance())
    {
        
    }

    std::string value = _readValue();

    // while (char ch = advance())
    // {
    //     if (ch=='\n')
    //     {
    //         lline += 1;
    //         ccol = 1;
    //         continue;
    //     }

    //     if (ch==' ')
    //     {
    //         continue;
    //     }
    
    //     else if (ch=='[')
    //     {
    //         std::string name = _readSection();
    //     }
    
    //     else if (isalpha(ch))
    //     {
    //         std::string label = _readLabel();
    //         std::string value = _readValue();

    //         mConf[label] = value;
    //     }
    // }
}


std::string ConfigParser::_readValue()
{
    std::string str = "";

    bool first = true;

    while (char ch = advance())
    {
        if (ch=='\"' && !first)
            break;

        else if (ch=='\n')
            break;

        first = false;

        str.push_back(ch);
    }
    printf("[_readValue] str=\"%s\"\n", str.c_str());

    while (check(' ')) {  }

    return str;
}



ConfigParser::ConfigParser( const char *path )
:   mPath(path), mIdx(0)
{
    std::ifstream stream(path, std::ifstream::binary);
    LogAsrt(stream.is_open(), "Could not open file \"%s\"", path);

    stream.seekg(0, std::ifstream::end);
    bbuf.resize(stream.tellg());

    stream.seekg(0, std::ifstream::beg);
    stream.read(bbuf.data(), bbuf.size());

    iidx  = 0;
    lline = 1;
    ccol  = 1;

    while (char ch = peek())
    {
        if (ch=='\n')
        {
            lline += 1;
            ccol = 1;
            continue;
        }

        if (ch==' ')
        {
            continue;
        }
    
        else if (ch=='[')
        {
            std::string name = _readSection();
        }
    
        else if (isalpha(ch))
        {
            std::string label = _readLabel();
            std::string value = _readValue();

            mConf[label] = value;
        }
    }
}

