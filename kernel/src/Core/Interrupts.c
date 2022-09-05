#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Drivers/Display.h"
#include "Drivers/PIC.h"

#include "Interrupts.h"
#include "Panic.h"
#include "IO.h"

#include "stdlib.h"

#define PANIC_HANDLER(name, message)                  \
    __attribute__((interrupt))                        \
    void name##_Handler(struct InterruptFrame* frame) \
    {                                                 \
        Panic(message);                               \
    }

PANIC_HANDLER(PageFault, "Page Fault");
PANIC_HANDLER(DoubleFault, "Double Fault");
PANIC_HANDLER(GeneralProtectionFault, "General Protection Fault");

__attribute__((interrupt)) void KeyboardInterrupt_Handler(struct InterruptFrame* frame)
{
    uint8_t scancode = inportb(0x60);
    char buffer[2];
    itoa(scancode, buffer, 16);

    static int y = 0;
    TextRenderer_RenderText("The keyboard was pressed, keyCode = 0x", 0, y);
    TextRenderer_RenderText(buffer, 39, y);
    y++;
}
