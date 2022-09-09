#include "IO.h"

void outportb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outportw(uint16_t port, uint16_t value)
{
    asm volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

void outportl(uint16_t port, uint32_t value)
{
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inportb(uint16_t port)
{
    uint8_t value;
    asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

uint16_t inportw(uint16_t port)
{
    uint16_t value;
    asm volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

uint32_t inportl(uint16_t port)
{
    uint32_t value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void IO_Wait()
{
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}
