#pragma once

#include <stdarg.h>
#include <stddef.h>

typedef struct FILE_t 
{
    char* pointer;
} FILE;

extern FILE* stdout;
extern FILE* stderr;
extern FILE* stdin;

void putchar(const char character);
void puts(const char* string);

void fputchar(FILE* file, const char character);
void fputs(FILE* file, const char* string);

void printf(const char* fmt, ...);
void fprintf(FILE* file, const char* fmt, ...);
void vfprintf(FILE* file, const char* fmt, va_list arg);
void vprintf(const char* fmt, va_list arg);

void sprintf(char* string, const char* fmt, ...);
void vsprintf(char* string, const char* fmt, va_list arg);

char getc(FILE* file);
void fgets(char* string, size_t size, FILE* stream);