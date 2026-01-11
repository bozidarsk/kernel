#include <stdint.h>
#include <stddef.h>

#include <elf.h>

#include "console.h"
#include "fonts.h"
#include "interrupts.h"
#include "ioport.h"
#include "cpuid.h"
#include "cpu.h"
#include "pci.h"
#include "pic.h"
#include "apic.h"
#include "ioapic.h"
#include "acpi.h"
#include "eh.h"

void int32(Registers regs, uint64_t error) 
{
	(void)regs;
	(void)error;
	console_writeline("timer");
}

__attribute__((noreturn))
void int3(Registers regs, uint64_t error) 
{
	(void)error;

	console_writeline("hit a software breakpoint:");
	const char* space = "  ";

	console_write("rip: 0x");
	console_write((void*)regs.rip);
	console_write(space);
	console_write("rflags: 0x");
	console_write((void*)regs.rflags);
	console_write(space);
	console_write("msr: 0x");
	console_writeline((void*)regs.msr);

	console_write("xmm0:  0x");
	console_write((void*)(regs.xmm0 >> 64));
	console_write((void*)regs.xmm0);
	console_write(space);
	console_write("xmm1:  0x");
	console_write((void*)(regs.xmm1 >> 64));
	console_writeline((void*)regs.xmm1);

	console_write("xmm2:  0x");
	console_write((void*)(regs.xmm2 >> 64));
	console_write((void*)regs.xmm2);
	console_write(space);
	console_write("xmm3:  0x");
	console_write((void*)(regs.xmm3 >> 64));
	console_writeline((void*)regs.xmm3);

	console_write("xmm4:  0x");
	console_write((void*)(regs.xmm4 >> 64));
	console_write((void*)regs.xmm4);
	console_write(space);
	console_write("xmm5:  0x");
	console_write((void*)(regs.xmm5 >> 64));
	console_writeline((void*)regs.xmm5);

	console_write("xmm6:  0x");
	console_write((void*)(regs.xmm6 >> 64));
	console_write((void*)regs.xmm6);
	console_write(space);
	console_write("xmm7:  0x");
	console_write((void*)(regs.xmm7 >> 64));
	console_writeline((void*)regs.xmm7);

	console_write("xmm8:  0x");
	console_write((void*)(regs.xmm8 >> 64));
	console_write((void*)regs.xmm8);
	console_write(space);
	console_write("xmm9:  0x");
	console_write((void*)(regs.xmm9 >> 64));
	console_writeline((void*)regs.xmm9);

	console_write("xmm10: 0x");
	console_write((void*)(regs.xmm10 >> 64));
	console_write((void*)regs.xmm10);
	console_write(space);
	console_write("xmm11: 0x");
	console_write((void*)(regs.xmm11 >> 64));
	console_writeline((void*)regs.xmm11);

	console_write("xmm12: 0x");
	console_write((void*)(regs.xmm12 >> 64));
	console_write((void*)regs.xmm12);
	console_write(space);
	console_write("xmm13: 0x");
	console_write((void*)(regs.xmm13 >> 64));
	console_writeline((void*)regs.xmm13);

	console_write("xmm14: 0x");
	console_write((void*)(regs.xmm14 >> 64));
	console_write((void*)regs.xmm14);
	console_write(space);
	console_write("xmm15: 0x");
	console_write((void*)(regs.xmm15 >> 64));
	console_writeline((void*)regs.xmm15);

	console_write("cr0: 0x");
	console_write((void*)regs.cr0);
	console_write(space);
	console_write("cr2: 0x");
	console_write((void*)regs.cr2);
	console_write(space);
	console_write("cr3: 0x");
	console_write((void*)regs.cr3);
	console_write(space);
	console_write("cr4: 0x");
	console_write((void*)regs.cr4);
	console_write(space);
	console_write("cr8: 0x");
	console_writeline((void*)regs.cr8);

	console_write("rax: 0x");
	console_write((void*)regs.rax);
	console_write(space);
	console_write("rbx: 0x");
	console_write((void*)regs.rbx);
	console_write(space);
	console_write("rcx: 0x");
	console_write((void*)regs.rcx);
	console_write(space);
	console_write("rdx: 0x");
	console_writeline((void*)regs.rdx);

	console_write("rsi: 0x");
	console_write((void*)regs.rsi);
	console_write(space);
	console_write("rdi: 0x");
	console_write((void*)regs.rdi);
	console_write(space);
	console_write("rbp: 0x");
	console_write((void*)regs.rbp);
	console_write(space);
	console_write("rsp: 0x");
	console_writeline((void*)regs.rsp);

	console_write("r8:  0x");
	console_write((void*)regs.r8);
	console_write(space);
	console_write("r9:  0x");
	console_writeline((void*)regs.r9);

	console_write("r10: 0x");
	console_write((void*)regs.r10);
	console_write(space);
	console_write("r11: 0x");
	console_writeline((void*)regs.r11);

	console_write("r12: 0x");
	console_write((void*)regs.r12);
	console_write(space);
	console_write("r13: 0x");
	console_writeline((void*)regs.r13);

	console_write("r14: 0x");
	console_write((void*)regs.r14);
	console_write(space);
	console_write("r15: 0x");
	console_writeline((void*)regs.r15);

	console_write("cs: ");
	console_write(regs.cs);
	console_write(space);
	console_write("ds: ");
	console_write(regs.ds);
	console_write(space);
	console_write("ss: ");
	console_write(regs.ss);
	console_write(space);
	console_write("es: ");
	console_write(regs.es);
	console_write(space);
	console_write("fs: ");
	console_write(regs.fs);
	console_write(space);
	console_write("gs: ");
	console_writeline(regs.gs);

	hlt();
}

void pci_print_device(CommonHeader header) 
{
	console_writeline((void*)header.subclass);
}

void kmain(Elf64_Ehdr* elf, XSDT* xsdt) 
{
	interrupts_disable();

	// console_set_char_width(8);
	// console_set_char_height(16);
	// console_set_width(framebuffer->width / 8);
	// console_set_height(framebuffer->height / 16);
	// console_set_pitch(framebuffer->pitch);
	// console_set_depth(framebuffer->depth);
	// console_set_framebuffer((void*)framebuffer->address);
	// console_set_video_mode(CONSOLE_VIDEO_MODE_VGA_GRAPHICS);
	// console_set_color_mode(CONSOLE_COLOR_MODE_R8G8B8);
	// console_set_bitmap(fonts_get_bitmap(FONT_NAME_W8H16));
	// console_clear();

	pic_remap(0x20);
	pic_disable();
	apic_initialize();

	console_write("apic base: ");
	console_writeline(apic_get_base());

	interrupts_initialize(8, 0);
	interrupts_set_handler(0x03, &int3);
	interrupts_set_handler(0x20, &int32);
	interrupts_enable();

	char vendor[12 + 1];
	cpuid_get_vendor(vendor);
	console_writeline(vendor);

	pci_enumerate_devices(&pci_print_device);
}
