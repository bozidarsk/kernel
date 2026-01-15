#include "kernel/eh.h"

__attribute__((noreturn))
void halt(void) { while (true) __asm__ volatile("hlt"); }

void breakpoint(void) { __asm__ volatile("int3"); }
