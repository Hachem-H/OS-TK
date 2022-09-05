#include "IO.h"

void outportb(uint16_t port, uint8_t value)
{
   asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inportb(uint16_t port)
{
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void IO_Wait()
{
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}