#include "Drivers/Display.h"
#include "Panic.h"

void Panic(const char* panicMessage)
{
    FrameBuffer_ClearColor(0xFF0000);
    TextRenderer_RenderText("==== KERNEL PANIC ====", 0, 0);
    TextRenderer_RenderText("ERR: ", 0, 1);
    TextRenderer_RenderText(panicMessage, 5, 1);
    for (;;);
}