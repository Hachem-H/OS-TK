#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Drivers/Keyboard.h"
#include "Drivers/Display.h"
#include "Drivers/PIC.h"

#include "Interrupts.h"
#include "Panic.h"
#include "IO.h"

#include <stdlib.h>

PANIC_HANDLER(PageFault, "Page Fault detected");
PANIC_HANDLER(DoubleFault, "Double Fault detected");
PANIC_HANDLER(GeneralProtectionFault, "General Protection Fault detected");

INTERRUPT_HANDLER(KeyboardInterrupt)
{
    uint8_t scanCode = inportb(0x60);
    KeyboardHandler(scanCode);
    RemapPIC();
}