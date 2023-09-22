#include "unsga.h"

#ifdef _WIN32
    #define EXPORT _declspec(dllexport)
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

extern "C"
{
    EXPORT void _cdecl create(void ** optimizor);
    EXPORT void _cdecl configurate();
    EXPORT void _free(void** optimizor);
}
