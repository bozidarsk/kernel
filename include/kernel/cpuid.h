#ifndef CPUID_H
#define CPUID_H

#include <stdint.h>

typedef enum : uint64_t
{
	CPUID_FEATURES_SSE3         = 1ul << 0,
	CPUID_FEATURES_PCLMUL       = 1ul << 1,
	CPUID_FEATURES_DTES64       = 1ul << 2,
	CPUID_FEATURES_MONITOR      = 1ul << 3,
	CPUID_FEATURES_DSCPL        = 1ul << 4,
	CPUID_FEATURES_VMX          = 1ul << 5,
	CPUID_FEATURES_SMX          = 1ul << 6,
	CPUID_FEATURES_EST          = 1ul << 7,
	CPUID_FEATURES_TM2          = 1ul << 8,
	CPUID_FEATURES_SSSE3        = 1ul << 9,
	CPUID_FEATURES_CID          = 1ul << 10,
	CPUID_FEATURES_SDBG         = 1ul << 11,
	CPUID_FEATURES_FMA          = 1ul << 12,
	CPUID_FEATURES_CX16         = 1ul << 13,
	CPUID_FEATURES_XTPR         = 1ul << 14,
	CPUID_FEATURES_PDCM         = 1ul << 15,
	CPUID_FEATURES_PCID         = 1ul << 17,
	CPUID_FEATURES_DCA          = 1ul << 18,
	CPUID_FEATURES_SSE41        = 1ul << 19,
	CPUID_FEATURES_SSE42        = 1ul << 20,
	CPUID_FEATURES_X2APIC       = 1ul << 21,
	CPUID_FEATURES_MOVBE        = 1ul << 22,
	CPUID_FEATURES_POPCNT       = 1ul << 23,
	CPUID_FEATURES_TSCDEAD      = 1ul << 24,
	CPUID_FEATURES_AES          = 1ul << 25,
	CPUID_FEATURES_XSAVE        = 1ul << 26,
	CPUID_FEATURES_OSXSAVE      = 1ul << 27,
	CPUID_FEATURES_AVX          = 1ul << 28,
	CPUID_FEATURES_F16C         = 1ul << 29,
	CPUID_FEATURES_RDRAND       = 1ul << 30,
	CPUID_FEATURES_HYPERVISOR   = 1ul << 31,

	CPUID_FEATURES_FPU          = 1ul << (32 + 0),
	CPUID_FEATURES_VME          = 1ul << (32 + 1),
	CPUID_FEATURES_DE           = 1ul << (32 + 2),
	CPUID_FEATURES_PSE          = 1ul << (32 + 3),
	CPUID_FEATURES_TSC          = 1ul << (32 + 4),
	CPUID_FEATURES_MSR          = 1ul << (32 + 5),
	CPUID_FEATURES_PAE          = 1ul << (32 + 6),
	CPUID_FEATURES_MCE          = 1ul << (32 + 7),
	CPUID_FEATURES_CX8          = 1ul << (32 + 8),
	CPUID_FEATURES_APIC         = 1ul << (32 + 9),
	CPUID_FEATURES_SEP          = 1ul << (32 + 11),
	CPUID_FEATURES_MTRR         = 1ul << (32 + 12),
	CPUID_FEATURES_PGE          = 1ul << (32 + 13),
	CPUID_FEATURES_MCA          = 1ul << (32 + 14),
	CPUID_FEATURES_CMOV         = 1ul << (32 + 15),
	CPUID_FEATURES_PAT          = 1ul << (32 + 16),
	CPUID_FEATURES_PSE36        = 1ul << (32 + 17),
	CPUID_FEATURES_PSN          = 1ul << (32 + 18),
	CPUID_FEATURES_CLFLUSH      = 1ul << (32 + 19),
	CPUID_FEATURES_DS           = 1ul << (32 + 21),
	CPUID_FEATURES_ACPI         = 1ul << (32 + 22),
	CPUID_FEATURES_MMX          = 1ul << (32 + 23),
	CPUID_FEATURES_FXSR         = 1ul << (32 + 24),
	CPUID_FEATURES_SSE          = 1ul << (32 + 25),
	CPUID_FEATURES_SSE2         = 1ul << (32 + 26),
	CPUID_FEATURES_SS           = 1ul << (32 + 27),
	CPUID_FEATURES_HTT          = 1ul << (32 + 28),
	CPUID_FEATURES_TM           = 1ul << (32 + 29),
	CPUID_FEATURES_IA64         = 1ul << (32 + 30),
	CPUID_FEATURES_PBE          = 1ul << (32 + 31),
} Features;

void cpuid(uint64_t command, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx);
void cpuid_get_vendor(char name[12 + 1]);
Features cpuid_get_features(void);
bool cpuid_has_features(Features features);

#endif
