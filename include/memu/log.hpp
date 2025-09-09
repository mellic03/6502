#pragma once

// namespace memu
// {
//     void logasrt(bool, const char*, ...);
// }

#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>


#define logasrt(Cd, FmSt, ...) \
{ \
    printf("[logasrt %s] " FmSt "\n", (Cd ? "PASS" : "FAIL"), __VA_ARGS__); \
    if (!(Cd)) { exit(1); } \
}

// void memu::logasrt(bool cond, const char *msg, ...)
// {
//     printf("[logasrt] %s: ", (cond ? "PASS" : "FAIL"));

//     va_list vlist;
//     va_start(vlist, msg);
//     vprintf(msg, vlist);
//     va_end(vlist);

//     assert(cond);
// }

