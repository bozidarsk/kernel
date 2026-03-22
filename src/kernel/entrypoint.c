#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <elf.h>

#include "kernel/interrupts.h"
#include "kernel/ioport.h"
#include "kernel/cpuid.h"
#include "kernel/cpu.h"
#include "kernel/pic.h"
#include "kernel/apic.h"
#include "kernel/ioapic.h"
#include "kernel/eh.h"

#include "kernel/drivers.h"
#include "bus/acpi.h"
#include "bus/pci.h"

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

	halt();
}

static void read_acpi_madte(MADTEntry* entry) 
{
	printf("madt entry: type=%d\n", entry->type);

	switch (entry->type) 
	{
		case MADT_ENTRY_TYPE_PROCESSOR_LOCAL_APIC:
			MADTEntryProcessorLocalAPIC* local = (MADTEntryProcessorLocalAPIC*)entry;
			printf("local apic: processorId=%d apicId=%d enabled=%d\n", local->processorId, local->apicId, local->processorEnabled);
			break;
		case MADT_ENTRY_TYPE_IO_APIC:
			MADTEntryIOAPIC* io = (MADTEntryIOAPIC*)entry;
			printf("io apic: id=%d address=%p globalSystemInterruptBase=%u\n", io->id, (uint64_t)io->address, io->globalSystemInterruptBase);
			break;
		default:
			break;
	}
}

static void read_acpi_sdt(SDT* header) 
{
	printf("sdt: type=%c%c%c%c\n",
		(header->type >> 0) & 0xff,
		(header->type >> 8) & 0xff,
		(header->type >> 16) & 0xff,
		(header->type >> 24) & 0xff
	);

	switch (header->type) 
	{
		case SDT_TYPE_APIC:
			MADT* madt = (MADT*)header;
			printf("madt: localAPICAddress=%p flags=%x\n", (uint64_t)madt->localAPICAddress, (uint32_t)madt->flags);
			acpi_enumerate_madt(madt, &read_acpi_madte);
			break;
		default:
			break;
	}
}

static void read_pci_device(PciDeviceHeader* header) 
{
	printf("pci device: type=%d vendor=%04x device=%04x class=%02x subclass=%04x mf=%d\n", header->type, header->vendor, header->device, header->class, header->subclass, header->hasMultipleFunctions);

	if (header->type != PCI_DEVICE_TYPE_GENERAL)
		return;

	Driver* driver = drivers_load((PciGeneralDevice*)header);

	if (!driver)
		return;

	printf("driver: type=%d bus=%d name=%s\n", driver->type, driver->bus, driver->name);
}

// static void setup_console(const Framebuffer* framebuffer) 
// {
// 	assert(framebuffer);

// 	console_set_char_width(8);
// 	console_set_char_height(16);
// 	console_set_width(framebuffer->width / 8);
// 	console_set_height(framebuffer->height / 16);
// 	console_set_pitch(framebuffer->pitch);
// 	console_set_depth(framebuffer->depth);
// 	console_set_framebuffer((void*)framebuffer->address);
// 	console_set_video_mode(CONSOLE_VIDEO_MODE_VGA_GRAPHICS);
// 	console_set_color_mode(CONSOLE_COLOR_MODE_R8G8B8);
// 	console_set_bitmap(console_fonts_get_bitmap(CONSOLE_FONT_NAME_W8H16));
// 	console_clear();
// }

static void setup_apic(const XSDT* xsdt) 
{
	assert(xsdt);

	pic_remap(PIC_OFFSET);
	pic_disable();

	acpi_enumerate_xsdt(xsdt, &read_acpi_sdt);

	apic_initialize();

	printf("local apic: base=%p\n", apic_get_base());
}

static void setup_interrupts(void) 
{
	interrupts_initialize(8, 0);
	interrupts_set_handler(0x03, &int3);
	interrupts_set_handler(0x20, &int32);
	interrupts_enable();
}

void kmain(Elf64_Ehdr* elf, XSDT* xsdt) 
{
	// setup_console(framebuffer);
	setup_apic(xsdt);
	setup_interrupts();

	pci_enumerate_devices(&read_pci_device);
}
