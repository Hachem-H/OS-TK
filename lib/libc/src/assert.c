#include "assert.h"
#include "stdio.h"

void _assert(const char* message, const char* file, int line)
{
    printf("Assertion failed: %s, file %s, line %d\n", message, file, line);
    __asm__("hlt");
}