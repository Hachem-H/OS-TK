#pragma once

typedef struct InterruptFrame_t InterruptFrame;

#define INTERRUPT_HANDLER(name)              \
    __attribute__((interrupt))               \
    void name##_Handler(InterruptFrame* frame)

#define PANIC_HANDLER(name, message) \
    INTERRUPT_HANDLER(name)          \
    {                                \
        Panic(message);              \
    }                                \

INTERRUPT_HANDLER(PageFault);
INTERRUPT_HANDLER(DoubleFault);
INTERRUPT_HANDLER(KeyboardInterrupt);
INTERRUPT_HANDLER(GeneralProtectionFault);