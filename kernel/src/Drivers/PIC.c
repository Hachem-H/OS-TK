#include "PIC.h"

#include "Core/IO.h"
#include "Core/Interrupts.h"

#include <stdint.h>

void RemapPIC()
{
    uint8_t a1 = inportb(PIC1_DATA);
    IO_Wait();
    uint8_t a2 = inportb(PIC2_DATA);
    IO_Wait();

    outportb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO_Wait();
    outportb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO_Wait();

    outportb(PIC1_DATA, 0x20);
    IO_Wait();
    outportb(PIC2_DATA, 0x28);
    IO_Wait();

    outportb(PIC1_DATA, 4);
    IO_Wait();
    outportb(PIC2_DATA, 2);
    IO_Wait();

    outportb(PIC1_DATA, ICW4_8086);
    IO_Wait();
    outportb(PIC2_DATA, ICW4_8086);
    IO_Wait();

    outportb(PIC1_DATA, a1);
    IO_Wait();
    outportb(PIC2_DATA, a2);
}

void PIC_EndSlave()
{
    outportb(PIC2_COMMAND, PIC_EOI);
    outportb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndMaster()
{
    outportb(PIC1_COMMAND, PIC_EOI);
}