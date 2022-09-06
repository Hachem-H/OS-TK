#pragma once

#include <stdarg.h>
#include <stddef.h>

typedef struct FILE_t 
{
    char* pointer;
} FILE;

#define FILENAME_MAX 4096
#define TMP_MAX      1226105504
#define FOPEN_MAX    16
#define EOF         -1

extern FILE* stdout;
extern FILE* stderr;
extern FILE* stdin;

void putchar(const char character);
void putc(const char character);
void puts(const char* string);
void fputc(FILE* file, const char character);
void fputs(FILE* file, const char* string);

void printf(const char* fmt, ...);
void fprintf(FILE* file, const char* fmt, ...);
void vfprintf(FILE* file, const char* fmt, va_list arg);
void vprintf(const char* fmt, va_list arg);
void sprintf(char* string, const char* fmt, ...);
void vsprintf(char* string, const char* fmt, va_list arg);

int scanf(const char* fmt, ...);
int fscanf(FILE* file, const char* fmt, ...);
int sscanf(char* string, const char* fmt, ...);

char getc(FILE* file);
char getchar();

void fgets(char* string, size_t size, FILE* stream);
size_t fread(void* pointer, size_t size, size_t nmemb, FILE* stream);