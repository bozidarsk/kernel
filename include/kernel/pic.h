#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC_MASTER_COMMAND 0x0020
#define PIC_MASTER_DATA 0x0021
#define PIC_SLAVE_COMMAND 0x00a0
#define PIC_SLAVE_DATA 0x00a1

#define PIC_ICW1_ICW4 0x01          /* Indicates that ICW4 will be present */
#define PIC_ICW1_SINGLE 0x02        /* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4 0x04     /* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL 0x08         /* Level triggered (edge) mode */
#define PIC_ICW1_INITIALIZE 0x10    /* Initialization - required! */

#define PIC_ICW4_8086 0x01          /* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO_EOI 0x02      /* Auto (normal) EOI */
#define PIC_ICW4_BUFFER_SLAVE 0x08  /* Buffered mode/slave */
#define PIC_ICW4_BUFFER_MASTER 0x0C /* Buffered mode/master */
#define PIC_ICW4_SFNM 0x10          /* Special fully nested (not) */

#define PIC_COMMAND_EOI 0x20
#define PIC_COMMAND_READ_IRR 0x0a
#define PIC_COMMAND_READ_ISR 0x0b

void pic_remap(int offset);
void pic_mask(uint8_t irq);
void pic_unmask(uint8_t irq);
void pic_enable(void);
void pic_disable(void);

#endif
