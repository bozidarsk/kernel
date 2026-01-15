#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);

void* memcpy(void* destination, const void* source, size_t size);
void* memset(void* destination, int value, size_t size);

#endif
