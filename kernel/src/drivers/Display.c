#include "Display.h"

void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font)
{
    GlobalTextRenderer.cursorPosition.x = 0;
    GlobalTextRenderer.cursorPosition.y = 0;

    GlobalTextRenderer.frameBuffer = frameBuffer;
    GlobalTextRenderer.color = 0xFFFFFFFF;
    GlobalTextRenderer.font = font;
}

void TextRenderer_RenderChar(char character, unsigned int xOff, unsigned int yOff)
{
    FrameBuffer* frameBuffer = GlobalTextRenderer.frameBuffer;
    PSFFont* font = GlobalTextRenderer.font;

    unsigned int* pixelPointer = (unsigned int*) frameBuffer->baseAddress;
    char* fontPointer = (char*)font->glyphBuffer+character*font->header->charSize;

    for (unsigned long y = yOff; y < yOff+16; ++y)
    {
        for (unsigned long x = xOff;  x < xOff+8; ++x)
            if ((*fontPointer & (0b10000000 >> (x-xOff))) > 0)
                pixelPointer[x+y*frameBuffer->pixelsPerScanLine] = GlobalTextRenderer.color;

        fontPointer++;
    }
}

void TextRenderer_RenderText(const char* string)
{
    const char* stringPointer = string;
    while (*stringPointer != 0x00)
    {
        TextRenderer_RenderChar(*stringPointer, GlobalTextRenderer.cursorPosition.x,
                                                GlobalTextRenderer.cursorPosition.y);
        GlobalTextRenderer.cursorPosition.x += 8;
        
        if(GlobalTextRenderer.cursorPosition.x+8 > GlobalTextRenderer.frameBuffer->width ||
           *stringPointer == '\n') 
        {
            GlobalTextRenderer.cursorPosition.x *= 0;
            GlobalTextRenderer.cursorPosition.y += 16;
        }

        stringPointer++;
    }
}
