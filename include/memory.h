#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

void* memcpy(void* destination, void* source, size_t size);

void* memset(void* destination, int32_t value, size_t size);

#endif
