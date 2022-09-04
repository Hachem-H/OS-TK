#pragma once

#ifdef NDEBUG
    #define assert(x)
#else
    #define assert(x) if (!(x)) { _assert(#x, __FILE__, __LINE__); }
#endif

void _assert(const char* message, const char* file, int line);