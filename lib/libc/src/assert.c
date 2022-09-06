#include "assert.h"
#include "stdio.h"

#include <Drivers/Display.h>
#include <Drivers/Console.h>

void _assert(const char* message, const char* file, int line)
{
    FrameBuffer_ClearColor(0xFF0000);
    Console_SetPosition(0, 0);

    puts("==== ASSERTION ====");
    printf("Assertion failed: %s, file %s, line %d\n", message, file, line);
    for (;;);
}