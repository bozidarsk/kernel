#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

ssize_t write(int fd, const void* buffer, size_t size);

size_t strlen(const char* str);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);

int atoi(const char* value);
long atol(const char* value);
long long atoll(const char* value);

void printf(const char* format, ...);
void sprintf(char* buffer, const char* format, ...);
void snprintf(char* buffer, size_t size, const char* format, ...);

void* memcpy(void* destination, const void* source, size_t size);
void* memset(void* destination, int32_t value, size_t size);

double fabs(double x);
double floor(double x);
double ceil(double x);

#endif
