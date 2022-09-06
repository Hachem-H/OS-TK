#include "Console.h"
#include "Display.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static struct
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;

    char* buffer;
} Console = {0};

static void PutChar(char character, uint32_t x, uint32_t y)
{
    uint32_t index = x + Console.width * y;
    Console.buffer[index] = character;
    TextRenderer_RenderChar(Console.buffer[index], x, y);
}

static void ClearRow(uint32_t y)
{
    for (uint32_t x = 0; x < Console.width; ++x)
        PutChar(' ', x, y);
}

void Console_Init()
{
    Console.x = 0;
    Console.y = 0;
    Console.width  = TextRenderer_GetWidth();
    Console.height = TextRenderer_GetHeight();
    Console.buffer = (char*)malloc(Console.width * Console.height);

    Console_Clear();
}

void Console_Destroy()
{
    Console_Clear();
    free(Console.buffer);
}

void Console_Clear()
{
    TextRenderer_Clear();
    for (uint32_t i = 0; i < Console.height*Console.width; ++i)
           Console.buffer[i] = ' ';
}


void Console_NewLine()
{
    for (uint32_t y = 1; y < Console.height; y++)
        for (uint32_t x = 0; x < Console.width; x++)
        {
            char character = Console.buffer[x + Console.width * y];
            TextRenderer_RenderChar(' ', x, y-1);
            PutChar(character, x, y-1);
        }

    ClearRow(Console.height-1);
    Console.x = 0;
}

void Console_PutChar(const char character)
{
    if (character == '\n')
    {
        Console_NewLine();
        return;
    }

    if (Console.x >= Console.width)
        Console_NewLine();
    
    PutChar(character, Console.x, Console.height-1);
    Console.x++;
}

void Console_PrintString(const char* string)
{
    for (size_t i = 0; i < strlen((char*)string); ++i)
        Console_PutChar(string[i]);
}

void Console_SetPosition(uint32_t x, uint32_t y)
{
    Console.x = x;
    Console.y = y;
}