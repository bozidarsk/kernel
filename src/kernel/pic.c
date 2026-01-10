#include "pic.h"
#include "ioport.h"

void pic_remap(int offset) 
{
	outb(PIC_MASTER_COMMAND, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4); // starts the initialization sequence (in cascade mode)
	ioport_wait();
	outb(PIC_SLAVE_COMMAND, PIC_ICW1_INITIALIZE | PIC_ICW1_ICW4);
	ioport_wait();

	outb(PIC_MASTER_DATA, offset); // ICW2: Master PIC vector offset
	ioport_wait();
	outb(PIC_SLAVE_DATA, offset + 8); // ICW2: Slave PIC vector offset
	ioport_wait();

	outb(PIC_MASTER_DATA, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	ioport_wait();
	outb(PIC_SLAVE_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
	ioport_wait();

	outb(PIC_MASTER_DATA, PIC_ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	ioport_wait();
	outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
	ioport_wait();
}

void pic_mask(uint8_t irq) 
{
	uint16_t port;

	if (irq < 8)
		port = PIC_MASTER_DATA;
	else 
	{
		port = PIC_SLAVE_DATA;
		irq -= 8;
	}

	outb(port, inb(port) | (1 << irq));		
}

void pic_unmask(uint8_t irq) 
{
	uint16_t port;

	if(irq < 8)
		port = PIC_MASTER_DATA;
	else 
	{
		port = PIC_SLAVE_DATA;
		irq -= 8;
	}

	outb(port, inb(port) & ~(1 << irq));		
}

void pic_enable(void) 
{
	outb(PIC_MASTER_DATA, 0);
	ioport_wait();
	outb(PIC_SLAVE_DATA, 0);
	ioport_wait();
}

void pic_disable(void) 
{
	outb(PIC_MASTER_DATA, 0xff);
	ioport_wait();
	outb(PIC_SLAVE_DATA, 0xff);
	ioport_wait();
}
