#include "Drivers/Display.h"
#include "Interrupts.h"
#include "stdbool.h"

__attribute__((interrupt)) void PageFault_Handler(InterruptFrame* frame) 
{
    FrameBuffer_ClearColor(0xFF0000);
    TextRenderer_RenderText("ERR: Page fault detected", 0, 0);
    while (true);
}