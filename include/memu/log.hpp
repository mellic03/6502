#pragma once

#include <cstdarg>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>



namespace memu::log
{
    void Info(const char*, ...);
    void Warn(const char*, ...);
    void Asrt(bool, const char*, ...);
}


/*
    #define LOG_INFO(FmSt, ...) \
    { \
        printf("[loginfo %s] " FmSt "\n", __VA_ARGS__); \
    }

    #define LOG_WARN(FmSt, ...) \
    { \
        printf("[loginfo %s] " FmSt "\n", __VA_ARGS__); \
    }
    printf("[logasrt %s] " FmSt "\n", (Cd ? "PASS" : "FAIL"), __VA_ARGS__); \
*/

#define LogAsrt(Cnd, FmStr, ...) \
{ \
    if (!(Cnd)) \
    { \
        printf("[Asrt FAIL] " FmStr, __VA_ARGS__); \
        assert((Cnd)); \
        assert(false); \
    } \
}





class syslog
{
private:
    static void _putIndent();
    static void _print( const char *fmt, ... );

public:
    static void pushIndent( int n=4 );
    static void popIndent( int n=4 );
    static int  getIndent();
    static void putIndent();
    static void vprint( const char *fmt, va_list vlist );
    static void print( const char *fmt, ... );
    static void println( const char *fmt, ... );

    template <typename... Args>
    syslog( const char *fmt, Args... args )
    {
        syslog::_putIndent();
        syslog::_print("[");
        syslog::_print(fmt, args...);
        syslog::_print("]\n");
        syslog::_putIndent();
        syslog::_print("{\n");
        pushIndent();
    }

    ~syslog()
    {
        popIndent();
        syslog::_putIndent();
        syslog::_print("}\n");
    }

    void operator()( const char *fmt, ... );
};

