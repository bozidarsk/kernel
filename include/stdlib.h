#ifndef STDLIB_H
#define STDLIB_H

int atoi(const char* value);
long atol(const char* value);
long long atoll(const char* value);

__attribute__((noreturn))
void abort();

#endif
