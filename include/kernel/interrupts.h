#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

typedef struct 
{
	unsigned __int128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
	uint64_t rip;
	uint64_t rflags;
	uint64_t msr;
	uint64_t cr0, cr2, cr3, cr4, cr8;
	uint64_t rax, rbx, rcx, rdx, rsi, rdi;
	uint64_t rbp, rsp;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint16_t cs, ds, ss, es, fs, gs;
} __attribute__((packed)) Registers;

typedef enum : uint8_t
{
	INTERRUPT_FLAGS_INTERRUPT = 0b00001110,
	INTERRUPT_FLAGS_TRAP      = 0b00001111,
	INTERRUPT_FLAGS_PRESENT   = 0b10000000,
	INTERRUPT_FLAGS_RING0     = 0b00000000,
	INTERRUPT_FLAGS_RING1     = 0b00100000,
	INTERRUPT_FLAGS_RING2     = 0b01000000,
	INTERRUPT_FLAGS_RING3     = 0b01100000,
} InterruptFlags;

typedef struct 
{
	uint16_t baseLow;
	uint16_t cs;
	uint8_t ist;
	InterruptFlags flags;
	uint16_t baseMid;
	uint32_t baseHigh;
	int32_t index;
} __attribute__((packed)) InterruptDescriptor;

typedef struct 
{
	uint16_t limit;
	InterruptDescriptor* base;
} __attribute__((packed)) InterruptDescriptorTable;

typedef void(*InterruptHandler)(Registers regs, uint64_t error);

void interrupts_enable();
void interrupts_disable();
void interrupts_load(const InterruptDescriptorTable* table);
void interrupts_initialize(int cs, int ist);
void interrupts_set_handler(int index, InterruptHandler handler);
InterruptHandler interrupts_get_handler(int index);

#endif
