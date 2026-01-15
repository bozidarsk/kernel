#include "kernel/eh.h"

__attribute__((noreturn))
void halt() { while (true) __asm__ volatile("hlt"); }

void breakpoint() { __asm__ volatile("int3"); }
