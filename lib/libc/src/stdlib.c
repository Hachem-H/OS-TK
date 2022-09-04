#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "ctype.h"

static unsigned long int next = 1;

char* itoa(int number, char* output, int base)
{
    if (base < 2 || base > 36) 
    { 
        *output = '\0'; 
        return output; 
    }

    char* backPointer  = output;
    char* frontPointer = output;

    char resChar;
    int resNumber;

    do {
        resNumber = number;
        number /= base;
        *backPointer++ = "zyxwvutsrqponmlkjihgfedcba"
                 "9876543210123456789"
                 "abcdefghijklmnopqrstuvwxyz" 
                 [35 + (resNumber - number * base)];
    } while (number);

    if (resNumber < 0) 
        *backPointer++ = '-';
    *backPointer-- = '\0';

    while(frontPointer < backPointer) 
    {
        resChar = *backPointer;
        *backPointer--= *frontPointer;
        *frontPointer++ = resChar;
    }
    return output;
}

int atoi(char* string)
{
    int result = 0;
    for (char* pointer = string; *pointer != 0; pointer++)
        result = result * 10 + *pointer - '0';
    return result;
}

long int atol(char* string)
{
    long int result = 0;
    for (char* pointer = string; *pointer != 0; pointer++)
        result = result * 10 + *pointer - '0';
    return result;
}

double atof(char* string)
{
    double output = 0.0;
    char character = 0;
    int e = 0;

    while ((character = *string++) != 0 && 
           (character >= '0' && 
            character <= '9'))
        output = output * 10.0 + character - '0';

    if (character == '.')
        while ((character = *string++) != 0 && 
               (character >= '0' && 
                character <= '9'))
        {
            output = output * 10.0 + character - '0';
            e = e-1;
        }

    if (character == 'e' || 
        character == 'E') 
    {
        int sign = 1;
        int i = 0;

        if (character == '+')
            character = *string++;
        else if (character == '-') 
        {
            character = *string++;
            sign = -1;
        }

        while (character >= '0' && character <= '9')
        {
            i = i * 10 + character - '0';
            character = *string++;
        }
        e += i * sign;
    }

    while (e > 0) 
    {
        output *= 10.0;
        e--;
    }

    while (e < 0) 
    {
        output /= 10.0;
        e++;
    }

    return output;
}

int abs(int x)
{
    if (x < 0)
        return -x;
    return x;
}

long int labs(long int x)
{
    if (x < 0)
        return -x;
    return x;
}

div_t div(int number, int denom)
{
    div_t result;
    result.quot = number / denom;
    result.rem = number % denom;

    if (number >= 0 && result.rem < 0)
    {
        ++result.quot;
        result.rem -= denom;
    }

    return result;
}

ldiv_t ldiv(long int number, long int denom)
{
    ldiv_t result;
    result.quot = number / denom;
    result.rem = number % denom;

    if (number >= 0 && result.rem < 0)
    {
        ++result.quot;
        result.rem -= denom;
    }

    return result;
}

int rand()
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % RAND_MAX;
}

void srand(unsigned int seed)
{
    next = seed;
}
