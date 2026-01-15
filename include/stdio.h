#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

ssize_t write(int fd, const void* buffer, size_t size);

void printf(const char* format, ...);
void sprintf(char* buffer, const char* format, ...);
void snprintf(char* buffer, size_t size, const char* format, ...);

#endif