#pragma once

#define RAND_MAX 32767

#include "stddef.h"

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

void* malloc(size_t size);
void free(void* address);