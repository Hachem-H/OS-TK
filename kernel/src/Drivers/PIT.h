#pragma once

#include <stdint.h>

void PIT_SetFrequency(uint64_t frequency);
void PIT_SetDivisor(uint16_t divisor);
uint64_t PIT_GetFrequency();

void PIT_SleepSeconds(double seconds);
void PIT_SleepMillis(uint64_t millis);
void PIT_Tick();

