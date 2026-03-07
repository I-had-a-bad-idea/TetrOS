#pragma once

#include "pic.h"
#include "isr.h"
#include "io.h"

#define PIC_REMAP_OFFSET	0x20

typedef struct Registers Registers;


typedef void (*IRQ_Handler)(Registers* regs);

void init_irq();
extern void irq_register_handler(int irq, IRQ_Handler handler);