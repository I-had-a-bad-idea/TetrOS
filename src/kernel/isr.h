#pragma once

#include "stdint.h"
#include "kernel.h"

struct registers
{
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
  uint32_t int_no, err_code;  // interrupt number and error code
  uint32_t eip, cs, eflags, useresp, ss;  // pushed by the processor automatically
};


#define ISR(n) \
void isr_##n() {};

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();

extern void common_isr_stub_handler();
extern void isr_handler(struct registers* regs);

extern void generic_isr_handler();
