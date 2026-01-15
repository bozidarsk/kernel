#include "kernel/cpu.h"

void cpu_set_msr(uint32_t command, uint32_t eax, uint32_t edx) 
{
	__asm__ volatile("wrmsr" : : "c" (command), "a" (eax), "d" (edx));
}

void cpu_get_msr(uint32_t command, uint32_t* eax, uint32_t* edx) 
{
	uint32_t a, d;

	__asm__ volatile("rdmsr" : "=a" (a), "=d" (d) : "c" (command));

	if (eax) *eax = a;
	if (edx) *edx = d;
}
