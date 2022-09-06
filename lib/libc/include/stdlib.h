#pragma once

#define RAND_MAX 32767

#include "stddef.h"

// abort
// exit
// atexit
//
// qsort

typedef struct 
{
    int quot;
    int rem;
} div_t;

typedef struct 
{
    long int quot;
    long int rem;
} ldiv_t;

char* itoa(int number, char* string, int base);
int atoi(char* string);

long int atol(char* string);
double atof(char* string);

int abs(int x);
long int labs(long int x);

div_t div(int number, int denom);
ldiv_t ldiv(long int number, long int denom);

int rand();
void srand(unsigned int seed);

void* bsearch (const void* key, const void* base0,
               size_t nmemb, size_t size,
              int (*)(const void*, const void*));

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void* pointer, size_t size);
void free(void* address);

unsigned long strtoul(const char* s, char** endp, int base);
long strtol(const char* s, char** endp, int base);
double strtod(const char* s, char** endp);