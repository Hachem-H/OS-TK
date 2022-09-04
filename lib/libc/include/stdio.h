#pragma once
#include <stdarg.h>

void putchar(const char character);
void puts(const char* string);

void printf(const char* fmt, ...);
void vprintf(const char* fmt, va_list arg);

void sprintf(char* string, const char* fmt, ...);
void vsprintf(char* string, const char* fmt, va_list arg);
