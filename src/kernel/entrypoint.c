#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include <elf.h>

#include "kernel/console.h"
#include "kernel/fonts.h"
#include "kernel/interrupts.h"
#include "kernel/ioport.h"
#include "kernel/cpuid.h"
#include "kernel/cpu.h"
#include "kernel/pci.h"
#include "kernel/pic.h"
#include "kernel/apic.h"
#include "kernel/ioapic.h"
#include "kernel/acpi.h"
#include "kernel/eh.h"

void int32(Registers regs, uint64_t error) 
{
	(void)regs;
	(void)error;
	printf("timer\n");
}

__attribute__((noreturn))
void int3(Registers regs, uint64_t error) 
{
	(void)error;

	printf("hit a software breakpoint:\n");

	printf("rip: 0x%.16llx  rflags: 0x%.16llx  msr: 0x%.16llx\n", regs.rip, regs.rflags, regs.msr);

	printf("xmm0:  0x%.16llx%.16llx  xmm1:  0x%.16llx%.16llx\n", regs.xmm0 >> 64, regs.xmm0, regs.xmm1 >> 64, regs.xmm1);
	printf("xmm2:  0x%.16llx%.16llx  xmm3:  0x%.16llx%.16llx\n", regs.xmm2 >> 64, regs.xmm2, regs.xmm3 >> 64, regs.xmm3);
	printf("xmm4:  0x%.16llx%.16llx  xmm5:  0x%.16llx%.16llx\n", regs.xmm4 >> 64, regs.xmm4, regs.xmm5 >> 64, regs.xmm5);
	printf("xmm6:  0x%.16llx%.16llx  xmm7:  0x%.16llx%.16llx\n", regs.xmm6 >> 64, regs.xmm6, regs.xmm7 >> 64, regs.xmm7);
	printf("xmm8:  0x%.16llx%.16llx  xmm9:  0x%.16llx%.16llx\n", regs.xmm8 >> 64, regs.xmm8, regs.xmm9 >> 64, regs.xmm9);
	printf("xmm10: 0x%.16llx%.16llx  xmm11: 0x%.16llx%.16llx\n", regs.xmm10 >> 64, regs.xmm10, regs.xmm11 >> 64, regs.xmm11);
	printf("xmm12: 0x%.16llx%.16llx  xmm13: 0x%.16llx%.16llx\n", regs.xmm12 >> 64, regs.xmm12, regs.xmm13 >> 64, regs.xmm13);
	printf("xmm14: 0x%.16llx%.16llx  xmm15: 0x%.16llx%.16llx\n", regs.xmm14 >> 64, regs.xmm14, regs.xmm15 >> 64, regs.xmm15);

	printf("cr0: 0x%.16llx  cr2: 0x%.16llx  cr3: 0x%.16llx  cr4: 0x%.16llx  cr8: 0x%.16llx\n", regs.cr0, regs.cr2, regs.cr3, regs.cr4, regs.cr8);

	printf("rax: 0x%.16llx  rbx: 0x%.16llx  rcx: 0x%.16llx  rdx: 0x%.16llx\n", regs.rax, regs.rbx, regs.rcx, regs.rdx);
	printf("rsi: 0x%.16llx  rdi: 0x%.16llx  rbp: 0x%.16llx  rsp: 0x%.16llx\n", regs.rsi, regs.rdi, regs.rbp, regs.rsp);

	printf("r8:  0x%.16llx  r9:  0x%.16llx\n", regs.r8, regs.r9);
	printf("r10: 0x%.16llx  r11: 0x%.16llx\n", regs.r10, regs.r11);
	printf("r12: 0x%.16llx  r13: 0x%.16llx\n", regs.r12, regs.r13);
	printf("r14: 0x%.16llx  r15: 0x%.16llx\n", regs.r14, regs.r15);

	printf("cs: %i  ds: %i  ss: %i  es: %i  fs: %i\n", regs.cs, regs.ds, regs.ss, regs.es, regs.fs);

	hlt();
}

void pci_print_device(CommonHeader header) 
{
	printf("%p\n", header.subclass);
}

void kmain(Elf64_Ehdr* elf, XSDT* xsdt) 
{
	interrupts_disable();

	pic_remap(0x20);
	pic_enable();

	interrupts_initialize(8, 0);
	interrupts_set_handler(0x03, &int3);
	interrupts_set_handler(0x20, &int32);
	interrupts_enable();

	char vendor[12 + 1];
	cpuid_get_vendor(vendor);
	printf("%s\n", vendor);
}
