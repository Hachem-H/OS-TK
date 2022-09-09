#include "PIT.h"
#include "Core/IO.h"

#define BASE_FREQ 1193182

static double TimeSinceBoot = 0;
static uint16_t Divisor = 65535;

void PIT_SetFrequency(uint64_t frequency)
{
    PIT_SetDivisor(BASE_FREQ/frequency);
}

void PIT_SetDivisor(uint16_t divisor)
{
    if (divisor < 100)
        divisor = 100;

    Divisor = divisor;
    outportb(0x40, (uint8_t)(divisor & 0x00FF));
    IO_Wait();
    outportb(0x40, (uint8_t)((divisor & 0xFF00) >> 8));
}

uint64_t PIT_GetFrequency()
{
    return BASE_FREQ/Divisor;
}

void PIT_SleepSeconds(double seconds)
{
    double startTime = TimeSinceBoot;
    while (TimeSinceBoot < startTime + seconds)
        asm("hlt");
}

void PIT_SleepMillis(uint64_t millis)
{
    PIT_SleepSeconds((double)millis/1000);
}

void PIT_Tick()
{
    TimeSinceBoot += 1/(double)PIT_GetFrequency();
}
