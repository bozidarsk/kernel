default abs

global _start

extern kmain
extern memcpy
extern STACKTOP

section .stack
section .framebuffer

section .rodata

gdt:
	dq 0 ; null entry
	.code_segment: equ $ - gdt
		dq (1 << 41) | (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Access.RW | Access.E | Access.S | Access.P | Flags.L
	.descriptor:
		dw $ - gdt - 1
		dq gdt

section .bss

alignb 4096
page_table_l4: resb 4096
page_table_l3: resb 4096
page_table_l2: resb 4096
page_table_l1: resb 4096

section .text

setup_page_tables:
	mov rax, page_table_l3 
	or rax, 0b11
	mov [page_table_l4], rax

	mov rax, page_table_l2
	or rax, 0b11
	mov [page_table_l3], rax

	mov rax, page_table_l1
	or rax, 0b11
	mov [page_table_l2], rax

	mov rcx, 1
	.loop1G: ; rcx = 1; rcx < 512; rcx++
	mov rax, 1 * 1024 * 1024 * 1024
	mul rcx
	or rax, 0b10000011
	mov [page_table_l3 + rcx * 8], rax
	inc rcx
	cmp rcx, 512
	jb .loop1G

	mov rcx, 1
	.loop2M: ; rcx = 1; rcx < 512; rcx++
	mov rax, 2 * 1024 * 1024
	mul rcx
	or rax, 0b10000011
	mov [page_table_l2 + rcx * 8], rax
	inc rcx
	cmp rcx, 512
	jb .loop2M

	xor rcx, rcx
	.loop4K: ; rcx = 0; rcx < 512; rcx++
	mov rax, 4 * 1024
	mul rcx
	test rcx, rcx
	jz .nopresent2K
	or rax, 1
	.nopresent2K:
	or rax, 0b10000010
	mov [page_table_l1 + rcx * 8], rax
	inc rcx
	cmp rcx, 512
	jb .loop4K

	ret

; void _start(Elf64_Ehdr* elf, uint64_t elfSize, XSDT* xsdt, uint64_t xsdtSize)
_start:
	cli

	mov rsp, STACKTOP

	lgdt [gdt.descriptor]

	push gdt.code_segment
	lea rax, [rel .reload]
	push rax
	retfq
	.reload:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	and rsp, ~0xf
	mov rbp, rsp
	sub rsp, 0x20
	mov [rbp - 8], rdi ; elf
	mov [rbp - 16], rsi ; elfSize
	mov [rbp - 24], rdx ; xsdt
	mov [rbp - 32], rcx ; xsdtSize

	call setup_page_tables
	mov rax, page_table_l4
	mov cr3, rax

	mov rax, cr0
	or eax, 1 << 1 ; MP - monitor co-processor
	and eax, ~(1 << 2) ; EM -  x87 FPU emulation
	or eax, 1 << 5 ; NE - numeric error
	or eax, 1 << 31 ; PG - paging
	mov cr0, rax

	mov rax, cr4
	or eax, 1 << 5 ; PAE - physical address extension
	or eax, 1 << 9 ; OSFXSR - operating system support for fxsave and fxrstor instructions
	or eax, 1 << 10 ; OSXMMEXCPT - operating system support for unmasked simd floating-point exceptions
	mov cr4, rax

	fninit

	sub rsp, [rbp - 16]
	and rsp, ~0xf
	mov rdi, rsp
	mov rsi, [rbp - 8]
	mov rdx, [rbp - 16]
	call memcpy
	mov [rbp - 8], rax

	sub rsp, [rbp - 32]
	and rsp, ~0xf
	mov rdi, rsp
	mov rsi, [rbp - 24]
	mov rdx, [rbp - 32]
	call memcpy
	mov [rbp - 24], rax

	mov rdi, [rbp - 8]
	mov rsi, [rbp - 24]

	xor rbp, rbp
	call kmain

	; hlt breaks interrupts
	jmp $
