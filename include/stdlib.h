#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>

void* memcpy(void* destination, void* source, size_t size);
void* memset(void* destination, int32_t value, size_t size);

double fabs(double x);
double floor(double x);
double ceil(double x);

#endif
