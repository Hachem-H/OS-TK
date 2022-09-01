#include "FrameBuffer.h"

typedef struct Position_t
{
    unsigned int x;
    unsigned int y;
} Position;

static void PutChar(FrameBuffer* frameBuffer, PSFFont* psfFont, 
                    unsigned int color, char character, 
                    unsigned int xOff, unsigned int yOff)
{
    unsigned int* pixelPointer = (unsigned int*) frameBuffer->baseAddress;
    char* fontPointer = psfFont->glyphBuffer+(character*psfFont->header->charSize);

    for (unsigned long y = yOff; y < yOff+16; ++y)
    {
        for (unsigned long x = xOff; x < xOff+8; ++x)
            if ((*fontPointer & (0b10000000 >> (x-xOff))) > 0)
                pixelPointer[x+y*frameBuffer->pixelsPerScanLine] = color;
        fontPointer++;
    }
}

static Position CursorPosition = { 0, 0 };
static void Print(FrameBuffer* frameBuffer, PSFFont* psfFont, 
           unsigned int colour, char* string)
{
    char* pointer = string;

    while(*pointer != 0x00)
    {
        PutChar(frameBuffer, psfFont, colour, *pointer, 
                CursorPosition.x, CursorPosition.y);
        CursorPosition.x += 8;

        if(CursorPosition.x + 8 > frameBuffer->width)
        {
            CursorPosition.x = 0;
            CursorPosition.y += 16;
        }

        pointer++;
    }
}

void _start(FrameBuffer* frameBuffer, PSFFont* font)
{
    Print(frameBuffer, font, 0xFFFFFFFF, "This should print using a custom font");
    CursorPosition.x *= 0;  // no new line so I have to manually update the
    CursorPosition.y += 16; // cursor position. +16 cause thats the char height.
    Print(frameBuffer, font, 0xFFFFFFFF, "And it does! First try.");
}