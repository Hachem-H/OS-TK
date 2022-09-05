#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Drivers/Display.h"

#include "Interrupts.h"
#include "Panic.h"

#define PANIC_HANDLER(name, message)                  \
    __attribute__((interrupt))                        \
    void name##_Handler(struct InterruptFrame* frame) \
    {                                                 \
        Panic(message);                               \
    }

PANIC_HANDLER(PageFault, "Page Fault detected");
PANIC_HANDLER(DoubleFault, "Double Fault detected");
PANIC_HANDLER(GeneralProtectionFault, "General Protection Fault detected");
