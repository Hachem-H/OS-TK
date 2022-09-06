#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "limits.h"
#include "ctype.h"

#include "Memory/Heap.h"

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

void free(void* address)
{
    HeapFree(address);
}

void* calloc(size_t nitems, size_t size)
{
    size_t bufferSize = nitems * size;
    void* buffer = malloc(bufferSize);
    memset(buffer, 0, bufferSize);
    return buffer;
}

void* realloc(void* pointer, size_t size)
{
    void* block = malloc(size);

    if (block == NULL)
        return pointer;
    
    if (pointer != NULL)
    {
        size_t toCopy;
        size_t originalSize = ((HeapSegmentHeader*)((unsigned char*)pointer - 
                               sizeof(HeapSegmentHeader)))->size;

        if (originalSize < size)
            toCopy = originalSize;
        else
            toCopy = size;

        memcpy(block, pointer, toCopy);          
        free(pointer);
    }

    return pointer;
}

void* malloc(size_t size)
{
    return HeapAllocate(size);
}

unsigned long strtoul(const char* source, char** endp, int base)
{
    register const char* s = source;
	register unsigned long accumulator;
	register int character;
	register unsigned long cutoff;
	register int negative = 0, any, cutLimit;

	do
    {
		character = *s++;
    } while (isspace(character));
    
	if (character == '-') 
    {
		negative = 1;
		character = *s++;
	} else if (character == '+')
		character = *s++;
	
    if ((base == 0 || base == 16) &&
	    character == '0' && (*s == 'x' || *s == 'X')) 
    {
		character = s[1];
		s += 2;
		base = 16;
	}

	if (base == 0)
		base = character == '0' ? 8 : 10;

	cutoff   = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutLimit = (unsigned long)ULONG_MAX % (unsigned long)base;
    
	for (accumulator = 0, any = 0; ; character = *s++) 
    {
		if (isdigit(character))
			character -= '0';
		else if (isalpha(character))
			character -= isupper(character) ? 'A' - 10 : 'a' - 10;
		else
			break;

		if (character >= base)
			break;

		if (any < 0 || accumulator > cutoff || (accumulator == cutoff && character > cutLimit))
			any = -1;
		else 
        {
			any = 1;
			accumulator *= base;
			accumulator += character;
		}
	}
	if (any < 0)
		accumulator = negative ? LONG_MIN : LONG_MAX;
	else if (negative)
		accumulator = -accumulator;

	if (endp != 0)
		*endp = (char *) (any ? s - 1 : s);
	return accumulator;
}

long strtol(const char* source, char** endp, int base)
{
    register const char *s = source;
	register unsigned long accumulator;
	register int character;
	register unsigned long cutoff;
	register int negative = 0, any, cutLimit;

	do
    {
		character = *s++;
    } while (isspace(character));
    
	if (character == '-') 
    {
		negative = 1;
		character = *s++;
	} else if (character == '+')
		character = *s++;
	
    if ((base == 0 || base == 16) &&
	    character == '0' && (*s == 'x' || *s == 'X')) 
    {
		character = s[1];
		s += 2;
		base = 16;
	}

	if (base == 0)
		base = character == '0' ? 8 : 10;

	cutoff = negative ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutLimit = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
    
	for (accumulator = 0, any = 0; ; character = *s++) 
    {
		if (isdigit(character))
			character -= '0';
		else if (isalpha(character))
			character -= isupper(character) ? 'A' - 10 : 'a' - 10;
		else
			break;

		if (character >= base)
			break;

		if (any < 0 || accumulator > cutoff || (accumulator == cutoff && character > cutLimit))
			any = -1;
		else 
        {
			any = 1;
			accumulator *= base;
			accumulator += character;
		}
	}
	if (any < 0)
		accumulator = negative ? LONG_MIN : LONG_MAX;
	else if (negative)
		accumulator = -accumulator;

	if (endp != 0)
		*endp = (char *) (any ? s - 1 : s);
	return accumulator;
}

double strtod(const char* str, char** ptr)
{
    char* pointer = (char*)str;

    if (ptr == (char **)0)
        return atof((char*)str);

    while (isspace(*pointer))
        ++pointer;
  
    if (*pointer == '+' || *pointer == '-')
        ++pointer;

    if    ((pointer[0] == 'i' || pointer[0] == 'I')
        && (pointer[1] == 'n' || pointer[1] == 'N')
        && (pointer[2] == 'f' || pointer[2] == 'F'))
    {
      if  ((pointer[3] == 'i' || pointer[3] == 'I')
	    && (pointer[4] == 'n' || pointer[4] == 'N')
	    && (pointer[5] == 'i' || pointer[5] == 'I')
	    && (pointer[6] == 't' || pointer[6] == 'T')
	    && (pointer[7] == 'y' || pointer[7] == 'Y'))
	    {
	        *ptr = pointer + 8;
	        return atof((char*)str);
	    }
        else
	    {
	        *ptr = pointer + 3;
	        return atof((char*)str);
	    }
    }

    if    ((pointer[0] == 'n' || pointer[0] == 'N')
        && (pointer[1] == 'a' || pointer[1] == 'A')
        && (pointer[2] == 'n' || pointer[2] == 'N'))
    {
        pointer += 3;
        if (*pointer == '(')
	    {
	        ++pointer;
	        while (*pointer != '\0' && *pointer != ')')
	            ++pointer;
	        if (*pointer == ')')
	        ++pointer;
	    }
      
        *ptr = pointer;
        return atof((char*)str);
    }

    if (isdigit(*pointer) || *pointer == '.')
    {
        int got_dot = 0;
        while (isdigit(*pointer) || (!got_dot && *pointer == '.'))
	    {
	        if (*pointer == '.')
	            got_dot = 1;
	        ++pointer;
	    }

    if (*pointer == 'e' || *pointer == 'E')
	{
	    int i;
	    i = 1;
	    if (pointer[i] == '+' || pointer[i] == '-')
	        ++i;
	    if (isdigit(pointer[i]))
	    {
	        while (isdigit(pointer[i]))
		        ++i;
	        *ptr = pointer + i;
	        return atof((char*)str);
	    }
	}
    
        *ptr = pointer;
        return atof((char*)str);
    }
    
    *ptr = (char*)str;
    return 0.0;
}

void* bsearch (const void* key, const void* base0,
               size_t nmemb, size_t size,
              int (*comparison)(const void*, const void*))
{
	const char* base = (const char*) base0;
	const void* pointer;
	int limit, cmp;

	for (limit = nmemb; limit != 0; limit >>= 1) 
    {
		pointer = base + (limit >> 1) * size;
		cmp = (*comparison)(key, pointer);

		if (cmp == 0)
			return (void*) pointer;

		if (cmp > 0)
        {
			base = (const char*) pointer + size;
			limit--;
		}
	}

	return (NULL);
}