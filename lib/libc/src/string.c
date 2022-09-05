#include "string.h"
#include "stddef.h"
#include "stdint.h"

size_t strlen(char* buffer)
{
    size_t size = 0;
    for (char* pointer = buffer; *pointer != 0x00; pointer++)
        size++;
    return size;
}

char* strcpy(char* dest, char* src)
{
    if (strlen(dest)+ 1!= strlen(src)+1)
        return (char*)0;
    for (int i = 0; i < strlen(src)+1; i++)
        dest[i] = src[i];
    return dest;
}

char* strncpy(char* dest, char* src, size_t size)
{
    for (int i = 0; i < size; i++)
        dest[i] = src[i];
    return dest;
}

char* strchr(char* source, int value)
{
    for (char* pointer = source; *pointer != 0x00; pointer++)
    {
        if (*pointer == value)
            return pointer;
    }
    return NULL;
}

char* strrchr(char* source, int value)
{
    char* ret = NULL;
    for (char* pointer = source; *pointer != 0x00; pointer++)
    {
        if (*pointer == value)
            ret = pointer;
    }

    return ret;
}


char* strpbrk(char* source, char* accept)
{
    if ((source == NULL) || (accept == NULL))
        return NULL;

    while (*source)
    {
        if (strchr(accept, *source))
            return source;
        else
            source++;
    }

    return NULL;
}

char* strstr(char* haystack, char* needle)
{
    if (*needle == 0x00)
        return haystack;

    for (int i = 0; i < strlen(haystack); i++)
    {
        if (*(haystack + i) == *needle)
        {
            char* pointer = strstr(haystack+i+1, needle+1);
            return pointer ? pointer - 1 : NULL;
        }
    }
    
    return NULL;
}

int strcmp(const char* source1, const char* source2)
{
    while (*source1 && (*source1 == *source2))
    {
        source1++;
        source2++;
    }

    return *(const unsigned char*)source1-
           *(const unsigned char*)source2;
}


int strspn(char* source, char* accept)
{
    unsigned int length = 0;
    char* pointer;
    
    for (length = 0; *source; source++, length++)
    {
        for (pointer = accept; 
            *pointer && *pointer != *source; 
             pointer++);

        if (!*pointer)
            break;
    }

    return length;
}

int strcspn(char* source, char* reject)
{
    unsigned int length = 0;

    if ((source == NULL) || (reject == NULL))
        return length;

    while (*source)
    {
        if (strchr(reject, *source))
            return length;
        else { source++;
               length++; }
    }

    return length;
}

int strncmp(const char* source1, const char* source2, size_t size)
{
    while (size && *source1 && (*source1 == *source2))
    {
        source1++;
        source2++;
        --size;
    }
   
    if (size == 0)
        return 0;
    else 
        return *(const unsigned char*)source1-
               *(const unsigned char*)source2;
}

void* memset(void* src, char value, size_t size)
{
    for (size_t i = 0; i < size; i++)
        *(uint8_t*)((uint64_t)src + i) = value;
    return src+size;
}

void* memcpy(void* dest, const void* src, size_t n)
{
    char* cdest = (char*) dest;
    char* csrc  = (char*) src;

    for (int i = 0; i < n; i++)
        cdest[i] = csrc[i];
    return dest;
}

void* memchr(void* src, int value, size_t size)
{
    unsigned char* pointer = (unsigned char*) src;
    unsigned char* found = NULL;
    while ((src != NULL) && (size--))
    {
        if (*pointer != (unsigned char)value)
            pointer++;
        else 
        {
            found = pointer;
            break;
        }
    }

    return found;
}

int memcmp(void* source1, void* source2, size_t size)
{
    unsigned char* pointer = source1;
    unsigned char* qointer = source2;
    int comparisonStatus = 0;

    if (source1 == source2)
        return comparisonStatus;

    while (size > 0)
    {
        if (*pointer != *qointer)
        {
            comparisonStatus = (*pointer > *qointer) ? 1 : -1;
            break;
        }

        size--;
        pointer++;
        qointer++;
    }

    return comparisonStatus;
}
