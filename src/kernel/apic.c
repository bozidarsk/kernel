#include "apic.h"
#include "cpu.h"
#include "eh.h"

void apic_set_base(void* value) 
{
	uint64_t apic = (uint64_t)value;

	uint32_t edx = 0;
	uint32_t eax = (apic & 0xfffff0000) | APIC_BASE_MSR_ENABLE;

	edx = (apic >> 32) & 0x0f;

	cpu_set_msr(APIC_BASE_MSR, eax, edx);
}

void* apic_get_base(void) 
{
	uint32_t eax, edx;
	cpu_get_msr(APIC_BASE_MSR, &eax, &edx);

	return (void*)(((uint64_t)eax & 0xfffff000) | (((uint64_t)edx & 0x0f) << 32));
}

uint32_t apic_register_read(int offset) 
{
	void* base = apic_get_base();

	return *(volatile uint32_t*)((volatile uint8_t*)base + offset);
}

void apic_register_write(int offset, uint32_t value) 
{
	void* base = apic_get_base();

	*(volatile uint32_t*)((volatile uint8_t*)base + offset) = value;
}

void apic_initialize(void) 
{
	if (!cpuid_has_features(CPUID_FEATURES_APIC))
		throw(InvalidOperationException, "Cpu does not support apic.");

	apic_set_base(apic_get_base());

	apic_register_write(APIC_REGISTER_OFFSET_SPURIOUS_INTERRUPT_VECTOR, apic_register_read(APIC_REGISTER_OFFSET_SPURIOUS_INTERRUPT_VECTOR) | 0x100 | 0xff);
}
