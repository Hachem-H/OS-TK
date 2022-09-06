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

INTERRUPT_HANDLER(DivisionByZero);
INTERRUPT_HANDLER(Debug);
INTERRUPT_HANDLER(NonMaskableInterrupt);
INTERRUPT_HANDLER(Breakpoint);
INTERRUPT_HANDLER(IntoDetectedOverflow);
INTERRUPT_HANDLER(OutofBounds);
INTERRUPT_HANDLER(InvalidOpcode);
INTERRUPT_HANDLER(NoCoprocessor);
INTERRUPT_HANDLER(DoubleFault);
INTERRUPT_HANDLER(CoprocessorSegmentOverrun);
INTERRUPT_HANDLER(BadTSS);
INTERRUPT_HANDLER(SegmentNotPresent);
INTERRUPT_HANDLER(StackFault);
INTERRUPT_HANDLER(GeneralProtectionFault);
INTERRUPT_HANDLER(PageFault);
INTERRUPT_HANDLER(UnknownInterrupt);
INTERRUPT_HANDLER(CoprocessorFault);
INTERRUPT_HANDLER(AlignmentCheck);
INTERRUPT_HANDLER(MachineCheck);

INTERRUPT_HANDLER(KeyboardInterrupt);
