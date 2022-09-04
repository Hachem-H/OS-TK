#pragma once

#include "stdint.h"
#include "stddef.h"

size_t strlen(char* buffer);
char* strcpy(char* dest, char* src);
char* strncpy(char* dest, char* src, size_t size);
char* strchr(char* source, int value);
char* strrchr(char* source, int value);
char* strstr(char* haystack, char* needle); 
char* strpbrk(char* source, char* accept);

int strcmp(const char* source1, const char* source2);
int strncmp(const char* source1, const char* source2, size_t size);

int strspn(char* source, char* accept);
int strcspn(char* source, char* reject);

void* memset(void* src, char value, size_t size);
void* memchr(void* src, int value, size_t size);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(void* source1, void* source2, size_t size);
