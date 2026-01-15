#include "kernel/interrupts.h"
#include "kernel/ioport.h"
#include "kernel/eh.h"

#define INTERRUPTS_COUNT 256

#define def_isr_error(n) __attribute__((naked)) static void isr##n(void) { __asm__ volatile("pushq $" #n); __asm__ volatile("jmp isr_common"); }
#define def_isr_noerror(n) __attribute__((naked)) static void isr##n(void) { __asm__ volatile("pushq $0"); __asm__ volatile("pushq $" #n); __asm__ volatile("jmp isr_common"); }

static InterruptHandler handlers[INTERRUPTS_COUNT] = {0};

__attribute__((aligned(16)))
static uint8_t fxsave[512];
static Registers regs;

__attribute__((naked))
__attribute__((used))
static void isr_common(void) 
{
	__asm__ volatile("mov %%rax, %0" : "=r" (regs.rax));
	__asm__ volatile("mov %%rbx, %0" : "=r" (regs.rbx));
	__asm__ volatile("mov %%rcx, %0" : "=r" (regs.rcx));
	__asm__ volatile("mov %%rdx, %0" : "=r" (regs.rdx));
	__asm__ volatile("mov %%rsi, %0" : "=r" (regs.rsi));
	__asm__ volatile("mov %%rdi, %0" : "=r" (regs.rdi));
	__asm__ volatile("mov %%r8, %0" : "=r" (regs.r8));
	__asm__ volatile("mov %%r9, %0" : "=r" (regs.r9));
	__asm__ volatile("mov %%r10, %0" : "=r" (regs.r10));
	__asm__ volatile("mov %%r11, %0" : "=r" (regs.r11));
	__asm__ volatile("mov %%r12, %0" : "=r" (regs.r12));
	__asm__ volatile("mov %%r13, %0" : "=r" (regs.r13));
	__asm__ volatile("mov %%r14, %0" : "=r" (regs.r14));
	__asm__ volatile("mov %%r15, %0" : "=r" (regs.r15));

	__asm__ volatile("fxsave64 (%0)"  : : "r" (fxsave) : "memory");

	__asm__ volatile("mov %%rbp, %0" : "=r"(regs.rbp));

	__asm__ volatile("mov %%rsp, %%rbp" : );

	// rbp + 0 == index
	// rbp + 8 == error
	// rbp + 16 == rip
	// rbp + 24 == cs
	// rbp + 32 == rflags
	// rbp + 40 == rsp
	// rbp + 48 == ss

	__asm__ volatile("mov 40(%%rbp), %0" : "=r" (regs.rsp));
	__asm__ volatile("mov 16(%%rbp), %0" : "=r" (regs.rip));
	__asm__ volatile("mov 32(%%rbp), %0" : "=r" (regs.rflags));

	__asm__ volatile("movlpd %%xmm0, (%0)\n\t" "movhpd %%xmm0, 8(%0)" : : "r" (&regs.xmm0) : "memory", "xmm0");
	__asm__ volatile("movlpd %%xmm1, (%0)\n\t" "movhpd %%xmm1, 8(%0)" : : "r" (&regs.xmm1) : "memory", "xmm1");
	__asm__ volatile("movlpd %%xmm2, (%0)\n\t" "movhpd %%xmm2, 8(%0)" : : "r" (&regs.xmm2) : "memory", "xmm2");
	__asm__ volatile("movlpd %%xmm3, (%0)\n\t" "movhpd %%xmm3, 8(%0)" : : "r" (&regs.xmm3) : "memory", "xmm3");
	__asm__ volatile("movlpd %%xmm4, (%0)\n\t" "movhpd %%xmm4, 8(%0)" : : "r" (&regs.xmm4) : "memory", "xmm4");
	__asm__ volatile("movlpd %%xmm5, (%0)\n\t" "movhpd %%xmm5, 8(%0)" : : "r" (&regs.xmm5) : "memory", "xmm5");
	__asm__ volatile("movlpd %%xmm6, (%0)\n\t" "movhpd %%xmm6, 8(%0)" : : "r" (&regs.xmm6) : "memory", "xmm6");
	__asm__ volatile("movlpd %%xmm7, (%0)\n\t" "movhpd %%xmm7, 8(%0)" : : "r" (&regs.xmm7) : "memory", "xmm7");
	__asm__ volatile("movlpd %%xmm8, (%0)\n\t" "movhpd %%xmm8, 8(%0)" : : "r" (&regs.xmm8) : "memory", "xmm8");
	__asm__ volatile("movlpd %%xmm9, (%0)\n\t" "movhpd %%xmm9, 8(%0)" : : "r" (&regs.xmm9) : "memory", "xmm9");
	__asm__ volatile("movlpd %%xmm10, (%0)\n\t" "movhpd %%xmm10, 8(%0)" : : "r" (&regs.xmm10) : "memory", "xmm10");
	__asm__ volatile("movlpd %%xmm11, (%0)\n\t" "movhpd %%xmm11, 8(%0)" : : "r" (&regs.xmm11) : "memory", "xmm11");
	__asm__ volatile("movlpd %%xmm12, (%0)\n\t" "movhpd %%xmm12, 8(%0)" : : "r" (&regs.xmm12) : "memory", "xmm12");
	__asm__ volatile("movlpd %%xmm13, (%0)\n\t" "movhpd %%xmm13, 8(%0)" : : "r" (&regs.xmm13) : "memory", "xmm13");
	__asm__ volatile("movlpd %%xmm14, (%0)\n\t" "movhpd %%xmm14, 8(%0)" : : "r" (&regs.xmm14) : "memory", "xmm14");
	__asm__ volatile("movlpd %%xmm15, (%0)\n\t" "movhpd %%xmm15, 8(%0)" : : "r" (&regs.xmm15) : "memory", "xmm15");

	__asm__ volatile("mov $0xc0000080, %%rcx" : );
	__asm__ volatile("rdmsr" : );
	__asm__ volatile("shl $32, %%rdx" : );
	__asm__ volatile("or %%rdx, %%rax" : );
	__asm__ volatile("mov %%rax, %0" : "=r" (regs.msr));

	__asm__ volatile("mov %%cr0, %0" : "=r" (regs.cr0));
	__asm__ volatile("mov %%cr2, %0" : "=r" (regs.cr2));
	__asm__ volatile("mov %%cr3, %0" : "=r" (regs.cr3));
	__asm__ volatile("mov %%cr4, %0" : "=r" (regs.cr4));
	__asm__ volatile("mov %%cr8, %0" : "=r" (regs.cr8));

	__asm__ volatile("mov 24(%%rbp), %0" : "=r" (regs.cs));
	__asm__ volatile("mov 48(%%rbp), %0" : "=r" (regs.ss));
	__asm__ volatile("mov %%ds, %0" : "=r" (regs.ds));
	__asm__ volatile("mov %%es, %0" : "=r" (regs.es));
	__asm__ volatile("mov %%fs, %0" : "=r" (regs.fs));
	__asm__ volatile("mov %%gs, %0" : "=r" (regs.gs));

	uint64_t index, error;
	__asm__ volatile("mov 0(%%rbp), %0" : "=r" (index));
	__asm__ volatile("mov 8(%%rbp), %0" : "=r" (error));

	if (handlers[index]) 
	{
		uint64_t size = (sizeof(Registers) | 0xf) + 1;
		__asm__ volatile("sub %0, %%rsp" : : "r" (size));
		handlers[index](regs, error);
	}

	int64_t irq = (int64_t)index - 0x20; // pic offset
	if (irq >= 0 && irq <= 15) 
	{
		if (irq > 7)
			outb(0x00a0 /* SLAVE_COMMAND */, 0x20 /* COMMAND_EOI */);

		outb(0x0020 /* MASTER_COMMAND */, 0x20 /* COMMAND_EOI */);
	}

	__asm__ volatile("movlpd (%0), %%xmm0\n\t" "movhpd 8(%0), %%xmm0" : : "r" (&regs.xmm0) : "memory", "xmm0");
	__asm__ volatile("movlpd (%0), %%xmm1\n\t" "movhpd 8(%0), %%xmm1" : : "r" (&regs.xmm1) : "memory", "xmm1");
	__asm__ volatile("movlpd (%0), %%xmm2\n\t" "movhpd 8(%0), %%xmm2" : : "r" (&regs.xmm2) : "memory", "xmm2");
	__asm__ volatile("movlpd (%0), %%xmm3\n\t" "movhpd 8(%0), %%xmm3" : : "r" (&regs.xmm3) : "memory", "xmm3");
	__asm__ volatile("movlpd (%0), %%xmm4\n\t" "movhpd 8(%0), %%xmm4" : : "r" (&regs.xmm4) : "memory", "xmm4");
	__asm__ volatile("movlpd (%0), %%xmm5\n\t" "movhpd 8(%0), %%xmm5" : : "r" (&regs.xmm5) : "memory", "xmm5");
	__asm__ volatile("movlpd (%0), %%xmm6\n\t" "movhpd 8(%0), %%xmm6" : : "r" (&regs.xmm6) : "memory", "xmm6");
	__asm__ volatile("movlpd (%0), %%xmm7\n\t" "movhpd 8(%0), %%xmm7" : : "r" (&regs.xmm7) : "memory", "xmm7");
	__asm__ volatile("movlpd (%0), %%xmm8\n\t" "movhpd 8(%0), %%xmm8" : : "r" (&regs.xmm8) : "memory", "xmm8");
	__asm__ volatile("movlpd (%0), %%xmm9\n\t" "movhpd 8(%0), %%xmm9" : : "r" (&regs.xmm9) : "memory", "xmm9");
	__asm__ volatile("movlpd (%0), %%xmm10\n\t" "movhpd 8(%0), %%xmm10" : : "r" (&regs.xmm10) : "memory", "xmm10");
	__asm__ volatile("movlpd (%0), %%xmm11\n\t" "movhpd 8(%0), %%xmm11" : : "r" (&regs.xmm11) : "memory", "xmm11");
	__asm__ volatile("movlpd (%0), %%xmm12\n\t" "movhpd 8(%0), %%xmm12" : : "r" (&regs.xmm12) : "memory", "xmm12");
	__asm__ volatile("movlpd (%0), %%xmm13\n\t" "movhpd 8(%0), %%xmm13" : : "r" (&regs.xmm13) : "memory", "xmm13");
	__asm__ volatile("movlpd (%0), %%xmm14\n\t" "movhpd 8(%0), %%xmm14" : : "r" (&regs.xmm14) : "memory", "xmm14");
	__asm__ volatile("movlpd (%0), %%xmm15\n\t" "movhpd 8(%0), %%xmm15" : : "r" (&regs.xmm15) : "memory", "xmm15");

	__asm__ volatile("fxrstor64 (%0)"  : : "r" (fxsave) : "memory");

	__asm__ volatile("mov %%rbp, %%rsp" : );
	__asm__ volatile("add $16, %%rsp" : ); // index + error

	__asm__ volatile("mov %0, %%r15" : : "r" (regs.r15));
	__asm__ volatile("mov %0, %%r14" : : "r" (regs.r14));
	__asm__ volatile("mov %0, %%r13" : : "r" (regs.r13));
	__asm__ volatile("mov %0, %%r12" : : "r" (regs.r12));
	__asm__ volatile("mov %0, %%r11" : : "r" (regs.r11));
	__asm__ volatile("mov %0, %%r10" : : "r" (regs.r10));
	__asm__ volatile("mov %0, %%r9" : : "r" (regs.r9));
	__asm__ volatile("mov %0, %%r8" : : "r" (regs.r8));
	__asm__ volatile("mov %0, %%rdi" : : "r" (regs.rdi));
	__asm__ volatile("mov %0, %%rsi" : : "r" (regs.rsi));
	__asm__ volatile("mov %0, %%rdx" : : "r" (regs.rdx));
	__asm__ volatile("mov %0, %%rcx" : : "r" (regs.rcx));
	__asm__ volatile("mov %0, %%rbx" : : "r" (regs.rbx));
	__asm__ volatile("mov %0, %%rax" : : "r" (regs.rax));

	__asm__ volatile("iretq");
}

def_isr_noerror(0);
def_isr_noerror(1);
def_isr_noerror(2);
def_isr_noerror(3);
def_isr_noerror(4);
def_isr_noerror(5);
def_isr_noerror(6);
def_isr_noerror(7);
def_isr_error(8);
def_isr_noerror(9);
def_isr_error(10);
def_isr_error(11);
def_isr_error(12);
def_isr_error(13);
def_isr_error(14);
def_isr_noerror(15);
def_isr_noerror(16);
def_isr_error(17);
def_isr_noerror(18);
def_isr_noerror(19);
def_isr_noerror(20);
def_isr_noerror(21);
def_isr_noerror(22);
def_isr_noerror(23);
def_isr_noerror(24);
def_isr_noerror(25);
def_isr_noerror(26);
def_isr_noerror(27);
def_isr_noerror(28);
def_isr_noerror(29);
def_isr_error(30);
def_isr_noerror(31);
def_isr_noerror(32);
def_isr_noerror(33);
def_isr_noerror(34);
def_isr_noerror(35);
def_isr_noerror(36);
def_isr_noerror(37);
def_isr_noerror(38);
def_isr_noerror(39);
def_isr_noerror(40);
def_isr_noerror(41);
def_isr_noerror(42);
def_isr_noerror(43);
def_isr_noerror(44);
def_isr_noerror(45);
def_isr_noerror(46);
def_isr_noerror(47);
def_isr_noerror(48);
def_isr_noerror(49);
def_isr_noerror(50);
def_isr_noerror(51);
def_isr_noerror(52);
def_isr_noerror(53);
def_isr_noerror(54);
def_isr_noerror(55);
def_isr_noerror(56);
def_isr_noerror(57);
def_isr_noerror(58);
def_isr_noerror(59);
def_isr_noerror(60);
def_isr_noerror(61);
def_isr_noerror(62);
def_isr_noerror(63);
def_isr_noerror(64);
def_isr_noerror(65);
def_isr_noerror(66);
def_isr_noerror(67);
def_isr_noerror(68);
def_isr_noerror(69);
def_isr_noerror(70);
def_isr_noerror(71);
def_isr_noerror(72);
def_isr_noerror(73);
def_isr_noerror(74);
def_isr_noerror(75);
def_isr_noerror(76);
def_isr_noerror(77);
def_isr_noerror(78);
def_isr_noerror(79);
def_isr_noerror(80);
def_isr_noerror(81);
def_isr_noerror(82);
def_isr_noerror(83);
def_isr_noerror(84);
def_isr_noerror(85);
def_isr_noerror(86);
def_isr_noerror(87);
def_isr_noerror(88);
def_isr_noerror(89);
def_isr_noerror(90);
def_isr_noerror(91);
def_isr_noerror(92);
def_isr_noerror(93);
def_isr_noerror(94);
def_isr_noerror(95);
def_isr_noerror(96);
def_isr_noerror(97);
def_isr_noerror(98);
def_isr_noerror(99);
def_isr_noerror(100);
def_isr_noerror(101);
def_isr_noerror(102);
def_isr_noerror(103);
def_isr_noerror(104);
def_isr_noerror(105);
def_isr_noerror(106);
def_isr_noerror(107);
def_isr_noerror(108);
def_isr_noerror(109);
def_isr_noerror(110);
def_isr_noerror(111);
def_isr_noerror(112);
def_isr_noerror(113);
def_isr_noerror(114);
def_isr_noerror(115);
def_isr_noerror(116);
def_isr_noerror(117);
def_isr_noerror(118);
def_isr_noerror(119);
def_isr_noerror(120);
def_isr_noerror(121);
def_isr_noerror(122);
def_isr_noerror(123);
def_isr_noerror(124);
def_isr_noerror(125);
def_isr_noerror(126);
def_isr_noerror(127);
def_isr_noerror(128);
def_isr_noerror(129);
def_isr_noerror(130);
def_isr_noerror(131);
def_isr_noerror(132);
def_isr_noerror(133);
def_isr_noerror(134);
def_isr_noerror(135);
def_isr_noerror(136);
def_isr_noerror(137);
def_isr_noerror(138);
def_isr_noerror(139);
def_isr_noerror(140);
def_isr_noerror(141);
def_isr_noerror(142);
def_isr_noerror(143);
def_isr_noerror(144);
def_isr_noerror(145);
def_isr_noerror(146);
def_isr_noerror(147);
def_isr_noerror(148);
def_isr_noerror(149);
def_isr_noerror(150);
def_isr_noerror(151);
def_isr_noerror(152);
def_isr_noerror(153);
def_isr_noerror(154);
def_isr_noerror(155);
def_isr_noerror(156);
def_isr_noerror(157);
def_isr_noerror(158);
def_isr_noerror(159);
def_isr_noerror(160);
def_isr_noerror(161);
def_isr_noerror(162);
def_isr_noerror(163);
def_isr_noerror(164);
def_isr_noerror(165);
def_isr_noerror(166);
def_isr_noerror(167);
def_isr_noerror(168);
def_isr_noerror(169);
def_isr_noerror(170);
def_isr_noerror(171);
def_isr_noerror(172);
def_isr_noerror(173);
def_isr_noerror(174);
def_isr_noerror(175);
def_isr_noerror(176);
def_isr_noerror(177);
def_isr_noerror(178);
def_isr_noerror(179);
def_isr_noerror(180);
def_isr_noerror(181);
def_isr_noerror(182);
def_isr_noerror(183);
def_isr_noerror(184);
def_isr_noerror(185);
def_isr_noerror(186);
def_isr_noerror(187);
def_isr_noerror(188);
def_isr_noerror(189);
def_isr_noerror(190);
def_isr_noerror(191);
def_isr_noerror(192);
def_isr_noerror(193);
def_isr_noerror(194);
def_isr_noerror(195);
def_isr_noerror(196);
def_isr_noerror(197);
def_isr_noerror(198);
def_isr_noerror(199);
def_isr_noerror(200);
def_isr_noerror(201);
def_isr_noerror(202);
def_isr_noerror(203);
def_isr_noerror(204);
def_isr_noerror(205);
def_isr_noerror(206);
def_isr_noerror(207);
def_isr_noerror(208);
def_isr_noerror(209);
def_isr_noerror(210);
def_isr_noerror(211);
def_isr_noerror(212);
def_isr_noerror(213);
def_isr_noerror(214);
def_isr_noerror(215);
def_isr_noerror(216);
def_isr_noerror(217);
def_isr_noerror(218);
def_isr_noerror(219);
def_isr_noerror(220);
def_isr_noerror(221);
def_isr_noerror(222);
def_isr_noerror(223);
def_isr_noerror(224);
def_isr_noerror(225);
def_isr_noerror(226);
def_isr_noerror(227);
def_isr_noerror(228);
def_isr_noerror(229);
def_isr_noerror(230);
def_isr_noerror(231);
def_isr_noerror(232);
def_isr_noerror(233);
def_isr_noerror(234);
def_isr_noerror(235);
def_isr_noerror(236);
def_isr_noerror(237);
def_isr_noerror(238);
def_isr_noerror(239);
def_isr_noerror(240);
def_isr_noerror(241);
def_isr_noerror(242);
def_isr_noerror(243);
def_isr_noerror(244);
def_isr_noerror(245);
def_isr_noerror(246);
def_isr_noerror(247);
def_isr_noerror(248);
def_isr_noerror(249);
def_isr_noerror(250);
def_isr_noerror(251);
def_isr_noerror(252);
def_isr_noerror(253);
def_isr_noerror(254);
def_isr_noerror(255);

static void(*isrs[INTERRUPTS_COUNT])(void) = 
{
	&isr0,
	&isr1,
	&isr2,
	&isr3,
	&isr4,
	&isr5,
	&isr6,
	&isr7,
	&isr8,
	&isr9,
	&isr10,
	&isr11,
	&isr12,
	&isr13,
	&isr14,
	&isr15,
	&isr16,
	&isr17,
	&isr18,
	&isr19,
	&isr20,
	&isr21,
	&isr22,
	&isr23,
	&isr24,
	&isr25,
	&isr26,
	&isr27,
	&isr28,
	&isr29,
	&isr30,
	&isr31,
	&isr32,
	&isr33,
	&isr34,
	&isr35,
	&isr36,
	&isr37,
	&isr38,
	&isr39,
	&isr40,
	&isr41,
	&isr42,
	&isr43,
	&isr44,
	&isr45,
	&isr46,
	&isr47,
	&isr48,
	&isr49,
	&isr50,
	&isr51,
	&isr52,
	&isr53,
	&isr54,
	&isr55,
	&isr56,
	&isr57,
	&isr58,
	&isr59,
	&isr60,
	&isr61,
	&isr62,
	&isr63,
	&isr64,
	&isr65,
	&isr66,
	&isr67,
	&isr68,
	&isr69,
	&isr70,
	&isr71,
	&isr72,
	&isr73,
	&isr74,
	&isr75,
	&isr76,
	&isr77,
	&isr78,
	&isr79,
	&isr80,
	&isr81,
	&isr82,
	&isr83,
	&isr84,
	&isr85,
	&isr86,
	&isr87,
	&isr88,
	&isr89,
	&isr90,
	&isr91,
	&isr92,
	&isr93,
	&isr94,
	&isr95,
	&isr96,
	&isr97,
	&isr98,
	&isr99,
	&isr100,
	&isr101,
	&isr102,
	&isr103,
	&isr104,
	&isr105,
	&isr106,
	&isr107,
	&isr108,
	&isr109,
	&isr110,
	&isr111,
	&isr112,
	&isr113,
	&isr114,
	&isr115,
	&isr116,
	&isr117,
	&isr118,
	&isr119,
	&isr120,
	&isr121,
	&isr122,
	&isr123,
	&isr124,
	&isr125,
	&isr126,
	&isr127,
	&isr128,
	&isr129,
	&isr130,
	&isr131,
	&isr132,
	&isr133,
	&isr134,
	&isr135,
	&isr136,
	&isr137,
	&isr138,
	&isr139,
	&isr140,
	&isr141,
	&isr142,
	&isr143,
	&isr144,
	&isr145,
	&isr146,
	&isr147,
	&isr148,
	&isr149,
	&isr150,
	&isr151,
	&isr152,
	&isr153,
	&isr154,
	&isr155,
	&isr156,
	&isr157,
	&isr158,
	&isr159,
	&isr160,
	&isr161,
	&isr162,
	&isr163,
	&isr164,
	&isr165,
	&isr166,
	&isr167,
	&isr168,
	&isr169,
	&isr170,
	&isr171,
	&isr172,
	&isr173,
	&isr174,
	&isr175,
	&isr176,
	&isr177,
	&isr178,
	&isr179,
	&isr180,
	&isr181,
	&isr182,
	&isr183,
	&isr184,
	&isr185,
	&isr186,
	&isr187,
	&isr188,
	&isr189,
	&isr190,
	&isr191,
	&isr192,
	&isr193,
	&isr194,
	&isr195,
	&isr196,
	&isr197,
	&isr198,
	&isr199,
	&isr200,
	&isr201,
	&isr202,
	&isr203,
	&isr204,
	&isr205,
	&isr206,
	&isr207,
	&isr208,
	&isr209,
	&isr210,
	&isr211,
	&isr212,
	&isr213,
	&isr214,
	&isr215,
	&isr216,
	&isr217,
	&isr218,
	&isr219,
	&isr220,
	&isr221,
	&isr222,
	&isr223,
	&isr224,
	&isr225,
	&isr226,
	&isr227,
	&isr228,
	&isr229,
	&isr230,
	&isr231,
	&isr232,
	&isr233,
	&isr234,
	&isr235,
	&isr236,
	&isr237,
	&isr238,
	&isr239,
	&isr240,
	&isr241,
	&isr242,
	&isr243,
	&isr244,
	&isr245,
	&isr246,
	&isr247,
	&isr248,
	&isr249,
	&isr250,
	&isr251,
	&isr252,
	&isr253,
	&isr254,
	&isr255,
};

static InterruptDescriptor descriptors[INTERRUPTS_COUNT] = {0};
static InterruptDescriptorTable table = 
{
	.limit = sizeof(descriptors) - 1,
	.base = descriptors,
};

void interrupts_set_handler(int index, InterruptHandler handler) 
{
	if (index < 0 || index >= INTERRUPTS_COUNT)
		throw(ArgumentOutOfRangeException, "Index is out of range.");

	handlers[index] = handler;
}

InterruptHandler interrupts_get_handler(int index) 
{
	if (index < 0 || index >= INTERRUPTS_COUNT)
		throw(ArgumentOutOfRangeException, "Index is out of range.");

	return handlers[index];
}

void interrupts_initialize(int cs, int ist) 
{
	for (int i = 0; i < INTERRUPTS_COUNT; i++) 
	{
		descriptors[i].index = i;
		descriptors[i].flags = INTERRUPT_FLAGS_INTERRUPT | INTERRUPT_FLAGS_PRESENT;

		size_t isr = (size_t)isrs[i];
		descriptors[i].baseLow = (uint16_t)(isr & 0xffff);
		descriptors[i].baseMid = (uint16_t)((isr >> 16) & 0xffff);
		descriptors[i].baseHigh = (uint32_t)((isr >> 32) & 0xffffffff);

		descriptors[i].cs = cs;
		descriptors[i].ist = ist;
	}

	interrupts_load(&table);
}
