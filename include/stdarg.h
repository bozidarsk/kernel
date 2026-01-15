#ifndef STDARG_H
#define STDARG_H

#define va_start(...) __builtin_c23_va_start(__VA_ARGS__)
#define va_end(list) __builtin_va_end(list)
#define va_arg(list, type) __builtin_va_arg(list, type)
#define va_copy(dest, src) __builtin_va_copy(dest, src)

typedef __builtin_va_list va_list;

#endif
