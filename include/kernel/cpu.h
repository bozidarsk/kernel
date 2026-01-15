#ifndef CPU_H
#define CPU_H

#include "kernel/cpuid.h"

void cpu_set_msr(uint32_t command, uint32_t eax, uint32_t edx);
void cpu_get_msr(uint32_t command, uint32_t* eax, uint32_t* edx);

#endif
