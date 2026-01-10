ENTRY(_start)

PHDRS 
{
   stack PT_LOAD FLAGS (6);
   framebuffer PT_LOAD FLAGS (6);
   text PT_LOAD FLAGS (5);
   rodata PT_LOAD FLAGS (4);
   data PT_LOAD FLAGS (6);
}

SECTIONS 
{
	. = 4K; /* not present */

	. = 512 * 4K; /* first page is not present (page fault triggers before NullReferenceException and StackOverflowException) */

	. = 2M;

	.stack ALIGN(16) : 
	{
		KEEP(*(.stack))
	} :stack
	. += 3 * 2M;
	PROVIDE(STACKTOP = .);

	PROVIDE(CONSOLEBUFFER = .);
	.framebuffer : 
	{
		KEEP(*(.framebuffer))
	} :framebuffer
	. += 3 * 2M;

	.text : 
	{
		*(.text*)
	} :text

	.rodata : 
	{
		*(.rodata*)
		*(.rdata*)
	} :rodata

	.data : 
	{
		*(.data*)
		*(.eh_frame*)
	} :data

	.bss : 
	{
		*(.bss*)
	} :data

	PROVIDE(KERNELEND = .);
}
