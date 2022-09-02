#include "drivers/Display.h"

void _start(FrameBuffer* frameBuffer, PSFFont* font)
{
    TextRenderer_InitWith(frameBuffer, font);
    TextRenderer_RenderText("Hello World!\n");
    TextRenderer_RenderText("This is pretty cool\n");
}
