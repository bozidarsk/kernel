#include "kernel/cpuid.h"
#include "kernel/eh.h"

void cpuid(uint64_t command, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) 
{
	uint32_t a, b, c, d;

	__asm__ volatile("xor %%rcx, %%rcx" : );
	__asm__ volatile("cpuid" : "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (command));

	if (eax) *eax = a;
	if (ebx) *ebx = b;
	if (ecx) *ecx = c;
	if (edx) *edx = d;
}

void cpuid_get_vendor(char name[12 + 1]) 
{
	if (!name)
		throw(ArgumentNullException, "Name buffer cannot be null.");

	uint32_t eax, ebx, ecx, edx;
	cpuid(0, &eax, &ebx, &ecx, &edx);

	name[0] = (char)((ebx >> 0) & 0xff);
	name[1] = (char)((ebx >> 8) & 0xff);
	name[2] = (char)((ebx >> 16) & 0xff);
	name[3] = (char)((ebx >> 24) & 0xff);

	name[4] = (char)((edx >> 0) & 0xff);
	name[5] = (char)((edx >> 8) & 0xff);
	name[6] = (char)((edx >> 16) & 0xff);
	name[7] = (char)((edx >> 24) & 0xff);

	name[8] = (char)((ecx >> 0) & 0xff);
	name[9] = (char)((ecx >> 8) & 0xff);
	name[10] = (char)((ecx >> 16) & 0xff);
	name[11] = (char)((ecx >> 24) & 0xff);

	name[12] = 0;
}

Features cpuid_get_features(void) 
{
	uint32_t eax, ebx, ecx, edx;
	cpuid(1, &eax, &ebx, &ecx, &edx);
	return (uint64_t)ecx | ((uint64_t)edx << 32);
}

bool cpuid_has_features(Features features) { return (cpuid_get_features() & features) == features; }
