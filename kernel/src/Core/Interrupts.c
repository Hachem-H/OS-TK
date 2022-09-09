#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Drivers/Keyboard.h"
#include "Drivers/Display.h"
#include "Drivers/PIC.h"
#include "Drivers/PIT.h"

#include "Interrupts.h"
#include "Panic.h"
#include "IO.h"

#include <stdlib.h>

PANIC_HANDLER(DivisionByZero, "Division by zero detected");
PANIC_HANDLER(Debug, "Debug detected");
PANIC_HANDLER(NonMaskableInterrupt, "Non maskable interrupt detected");
PANIC_HANDLER(Breakpoint, "Breakpoint detected");
PANIC_HANDLER(IntoDetectedOverflow, "Detected overflow");
PANIC_HANDLER(OutofBounds, "Out of bound exception detected");
PANIC_HANDLER(InvalidOpcode, "Invalid OpCode");
PANIC_HANDLER(NoCoprocessor, "No coprocessor found");
PANIC_HANDLER(DoubleFault, "Double Fault detected");
PANIC_HANDLER(CoprocessorSegmentOverrun, "Coprocessor segment overrun");
PANIC_HANDLER(BadTSS, "Bad TSS detected");
PANIC_HANDLER(SegmentNotPresent, "Segment not present");
PANIC_HANDLER(StackFault, "Stack fault detected");
PANIC_HANDLER(GeneralProtectionFault, "General Protection Fault detected");
PANIC_HANDLER(PageFault, "Page fault detected");
PANIC_HANDLER(UnknownInterrupt, "Unknown Interrupt");
PANIC_HANDLER(CoprocessorFault, "Coprocessor Fault decteted");
PANIC_HANDLER(AlignmentCheck, "Alignment check failed");
PANIC_HANDLER(MachineCheck, "Machine check failed");

INTERRUPT_HANDLER(KeyboardInterrupt)
{
    uint8_t scanCode = inportb(0x60);
    KeyboardHandler(scanCode);
    PIC_EndMaster();
}

INTERRUPT_HANDLER(PITInterrupt)
{
    PIT_Tick();    
    PIC_EndMaster();
}
