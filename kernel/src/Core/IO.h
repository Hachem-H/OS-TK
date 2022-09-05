#pragma once

#include "stdint.h"

void outportb(uint16_t port, uint8_t value);
uint8_t inportb(uint16_t port);

void IO_Wait();