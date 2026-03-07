#pragma once

#include "stdint.h"
#include "io.h"

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */

void PIC_remap(uint8_t pic1_offset, uint8_t pic2_offset);
void PIC_send_end_of_interrupt(int irq);
void PIC_mask_all();
void PIC_unmask_all();
void PIC_mask(int irq);
void PIC_unmask(int irq);