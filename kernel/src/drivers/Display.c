#include "Display.h"

static TextRenderer GlobalTextRenderer;

void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font)
{
    GlobalTextRenderer.cursorPosition.x = 0;
    GlobalTextRenderer.cursorPosition.y = 0;

    GlobalTextRenderer.frameBuffer = frameBuffer;
    GlobalTextRenderer.color = 0xFFFFFF;
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
        if (*stringPointer != '\n')
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
void TextRenderer_SetFont(PSFFont* font)                    { GlobalTextRenderer.font = font;               }
void TextRenderer_SetColor(uint32_t color)                  { GlobalTextRenderer.color = color;             }
void TextRenderer_SetFrameBuffer(FrameBuffer* frameBuffer)  { GlobalTextRenderer.frameBuffer = frameBuffer; }
void TextRenderer_SetCursorPosition(uint32_t x, uint32_t y) { GlobalTextRenderer.cursorPosition.x = x; 
                                                              GlobalTextRenderer.cursorPosition.y = y;      }