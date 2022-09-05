#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Drivers/Display.h"
#include "Drivers/PIC.h"

#include "Interrupts.h"
#include "Panic.h"
#include "IO.h"

#include "stdlib.h"

PANIC_HANDLER(PageFault, "Page Fault detected");
PANIC_HANDLER(DoubleFault, "Double Fault detected");
PANIC_HANDLER(GeneralProtectionFault, "General Protection Fault detected");

INTERRUPT_HANDLER(KeyboardInterrupt)
{
    uint8_t scanCode = inportb(0x60);
    char buffer[2];
    itoa(scanCode, buffer, 16);
    
    static int y = 0;
    TextRenderer_RenderText("Keyboard was pressed, key code = 0x", 0, y);
    TextRenderer_RenderText(buffer, 35, y);
    y++;

    RemapPIC();
}